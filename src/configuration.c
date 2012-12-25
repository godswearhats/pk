/* 
 *  Copyright 2012 John Morrow
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <argp.h>
#include <stdlib.h>

#include "configuration.h"
#include "wrappers.h"

const char *argp_program_version = "fields 1.0";
const char *argp_program_bug_address = "<bug-fields@amoeba.org>";
static char args_doc[] = "[STRING...]";
static char doc[] = "A field extraction utility";

static struct argp_option options[] = {
    {"delimiters", 'd', "STRING", 0, "Characters used as input delimiters", 0},
    {"empty", 'e', 0, 0, "Allow empty fields", 0},
    {"exclude", 'E', 0, 0, "List of strings to exclude from output separated by :", 0},
    {"file", 'f', "FILE", 0, "Read input from file instead of stdin", 0},
    {"separator", 's', "STRING", 0, "Separator used in output text", 0},
    {0, 0, 0, 0, 0, 0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    CONFIGURATION *configuration = state->input;
    switch (key)
    {
    case 'd':
        configuration->delimiters = arg;
        break;
    case 'e':
        configuration->allow_empty_tokens = 1;
        break;
    case 'E':
        configuration->exclude = NULL;
        break;
    case 'f':
        configuration->file = arg;
        break;
    case 's':
        configuration->separator = arg;
        break;
    case ARGP_KEY_NO_ARGS:
        argp_usage(state);
        /*NOTREACHED*/
        break;
    case ARGP_KEY_ARG:
        configuration->fields = &state->argv[state->next - 1];
        state->next = state->argc;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

CONFIGURATION *configuration_new(int argc, char **argv)
{
    CONFIGURATION *self = MALLOC(CONFIGURATION);
    self->delimiters = "\t ";
    self->allow_empty_tokens = 0;
    self->fields = NULL;
    self->file = NULL;
    self->separator = " ";
    struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };
    argp_parse(&argp, argc, argv, 0, 0, self);
    return self;
}

void configuration_delete(CONFIGURATION *self)
{
    Free(self);
}
