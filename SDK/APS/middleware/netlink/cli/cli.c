/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "cli.h"
#include "diag_task.h"

int cli_do_multilevel_cmd(cli_command_t *cmd_tble, int argc, char *argv[])
{
    const cli_command_t *cmd_ptr;

    // pass token count and token to command handler*/
    for(cmd_ptr=cmd_tble; cmd_ptr->cmd; cmd_ptr++)
    {
        if(!strcmp(argv[0], cmd_ptr->cmd))
        {
            printf("\r\n");
            if (cmd_ptr->cmd_handle)
                cmd_ptr->cmd_handle(argc-1, &argv[1]);
            else
                cli_do_multilevel_cmd(cmd_ptr->sub, argc-1, &argv[1]);
            break;
        }
    }
    return 1;
}


#if defined(__SIMULATOR__)

#define CLI_MAX_BUFFER_SIZE         80   // CLI Buffer size (char)
#define CLI_PROMPT "OPL1000> "

//extern void CmdProcess(char *pbuf, int len);
extern char uart_getchar(void);
extern int kbhit(void);

/**
 * Static Global variables for CLI status:
 */
static char cmd_line[CLI_MAX_BUFFER_SIZE + 1]; // Extra space for terminating '\0'
static char cmd_back[CLI_MAX_BUFFER_SIZE + 1]; // Extra space for terminating '\0'

static uint8_t cmd_line_len = 0;
static uint8_t cursor_pos = 0;
static cli_cb_t *clicb_ptr;

static void cli_cmd_usage(void)
{
    const cli_command_t *cmd_ptr;

    for(cmd_ptr=clicb_ptr->cmd; cmd_ptr->cmd; cmd_ptr++) {
        printf("%-10s| %s\r\n",  cmd_ptr->cmd, cmd_ptr->cmd_usage);
    }
}

static int cli_do_cmd(void)
{
    int argc = 1;
    char *token;
    char *argv[CLI_MAX_CMD_ARGS];
    const cli_command_t *cmd_ptr;
    char *saveptr = cmd_line;

    /* get the first token */
    argv[0] = strtok_r(cmd_line, " ", &saveptr);
    clicb_ptr->tok[0] = argv[0];

    /* split the rest of the token until the end of the message */
    if (argv[0] != NULL) {
        while( ( (token = strtok_r(NULL, " ", &saveptr )) != NULL) && argc != CLI_MAX_CMD_ARGS) {
            argv[argc] = token;
            clicb_ptr->tok[argc] = token;
            argc++;
        }
    }

    // pass token count and token to command handler*/
#if 0
    for(cmd_ptr=clicb_ptr->cmd; cmd_ptr->cmd; cmd_ptr++)
    {
        if(!strcmp(argv[0], cmd_ptr->cmd))
        {
            printf("\r\n");
            if (cmd_ptr->cmd_handle)
                cmd_ptr->cmd_handle(argc, argv);
            break;
        }
    }
#else
    cli_do_multilevel_cmd(clicb_ptr->cmd, argc, argv);
#endif
    if (strcmp(argv[0], "?") == 0) {
        printf("\r\n");
        cli_cmd_usage();
    }

    return 1;
}

static void process_char_backspace(void)
{
    if (cursor_pos == 0) {
        return;
    }

    for (uint8_t i = cursor_pos; i < cmd_line_len; i++) {
        cmd_line[i - 1] = cmd_line[i];
    }

    printf("%c",0x08);
    printf("%c",0x20);
    printf("%c",0x08);

    cursor_pos--;
    cmd_line_len--;
}

static void process_enter_pressed(void)
{
    cmd_line[cmd_line_len] = '\0';
    memcpy(cmd_back, cmd_line, (CLI_MAX_BUFFER_SIZE + 1));

    cli_do_cmd();

    CmdProcess(cmd_back, cmd_line_len);

    cmd_line_len = 0;
    cursor_pos = 0;
}

static void process_char_insert(char ch)
{
    if (cmd_line_len >= CLI_MAX_BUFFER_SIZE) {
        return;
    }

    for (uint8_t i = cursor_pos; i < cmd_line_len; i++) {
        cmd_line[i + 1] = cmd_line[i];
    }

    cmd_line[cursor_pos] = ch;
    cursor_pos++;
    cmd_line_len++;
    printf("%c", ch); //input echo
}

static void process_char_delete(void)
{
    if (cursor_pos == cmd_line_len) {
        return;
    }

    for (uint8_t i = cursor_pos; i < cmd_line_len - 1; i++) {
        cmd_line[i] = cmd_line[i + 1];
    }

    cmd_line_len--;
}

void nl_cli_init(cli_cb_t *cli)
{
    printf("\n<<Start Command Line>>\nPress '?'for help......\n\n");
    printf("\n%s", CLI_PROMPT);

    clicb_ptr = cli; //initialize cli control block
    clicb_ptr->state = 1;

    cmd_line[0] = 0;
    cmd_line_len = 0;
    cursor_pos = 0;
}

void nl_cli_task(void)
{
    char ch;

    if (clicb_ptr->state == 0 ) {
        printf("CLI is not initialization !!!\n");
        return;
    }

    if(kbhit() == false) {
        return;
    }

    switch ( ch = uart_getchar() )
    {
        case 0x00: // Special Key, read again for real data
            break;

        case 0x08: // Backspace
        case 0x7f: // Del
            process_char_backspace();
            break;

        case 0x0d: /* Enter */
            process_enter_pressed();
            printf("\n%s", CLI_PROMPT);
            break;

        default: /* other characters */
            process_char_insert(ch);
        break;
    }
}
#endif
