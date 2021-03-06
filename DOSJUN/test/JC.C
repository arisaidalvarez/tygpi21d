/* I N C L U D E S /////////////////////////////////////////////////////// */

#include "jc.h"
#include "code.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* D E F I N E S ///////////////////////////////////////////////////////// */

#define JC_LINE_LENGTH	1000

/* P R O T O T Y P E S /////////////////////////////////////////////////// */

bool Tokenize_Code_String(char *source, jc_token *tokens, int *count);

/* F U N C T I O N S ///////////////////////////////////////////////////// */

jc_token_type Parse_Operator(char *string)
{
	if (!strncmp(string, "==", 2)) return ttEquals;
	if (!strncmp(string, "!=", 2)) return ttNotEqual;
	if (!strncmp(string, "<=", 2)) return ttLTE;
	if (!strncmp(string, ">=", 2)) return ttGTE;
	if (!strncmp(string, "<", 1)) return ttLT;
	if (!strncmp(string, ">", 1)) return ttGT;
	if (!strncmp(string, "=", 1)) return ttAssignment;

	return ttUnknown;
}

bool Is_Code_Keyword(char *string)
{
	if (streq(string, "Combat")) return true;
	if (streq(string, "Const")) return true;
	if (streq(string, "Else")) return true;
	if (streq(string, "ElseIf")) return true;
	if (streq(string, "EndIf")) return true;
	if (streq(string, "EndScript")) return true;
	if (streq(string, "EquipItem")) return true;
	if (streq(string, "GiveItem")) return true;
	if (streq(string, "Global")) return true;
	if (streq(string, "If")) return true;
	if (streq(string, "Include")) return true;
	if (streq(string, "Local")) return true;
	if (streq(string, "Music")) return true;
	if (streq(string, "PcSpeak")) return true;
	if (streq(string, "Return")) return true;
	if (streq(string, "Safe")) return true;
	if (streq(string, "Script")) return true;
	if (streq(string, "SetDanger")) return true;
	if (streq(string, "SetTileColour")) return true;
	if (streq(string, "SetTileDescription")) return true;
	if (streq(string, "SetTileThing")) return true;
	if (streq(string, "Teleport")) return true;
	if (streq(string, "Text")) return true;
	if (streq(string, "Unlock")) return true;

	return false;
}

internal_id Get_Internal_Id(char *string)
{
	if (streq(string, "Danger")) return internalDanger;
	if (streq(string, "Facing")) return internalFacing;
	if (streq(string, "X")) return internalX;
	if (streq(string, "Y")) return internalY;
	if (streq(string, "JustMoved")) return internalJustMoved;

	return internalInvalid;
}

char *Get_Token_Name(jc_token_type tt)
{
	switch (tt) {
		case ttUnknown: return "UNK";
		case ttComment: return "COM";
		case ttInternal: return "INT";
		case ttKeyword: return "KEY";
		case ttIdentifier: return "VAR";
		case ttString: return "STR";
		case ttNumber: return "NUM";
		case ttAssignment: return "SET";
		case ttEquals: return "EQU";
		case ttNotEqual: return "NEQ";
		case ttLT: return "LT ";
		case ttLTE: return "LTE";
		case ttGT: return "GT ";
		case ttGTE: return "GTE";

		default: return "???";
	}
}

int Compile_JC(jc_parser *parser, char *filename, bool toplevel)
{
	bool success;
	char line[JC_LINE_LENGTH];
	jc_token *tokens;
	int count,
		i,
		err = 0,
		line_no = 0;
	FILE *fp;

	printf("%s:\n", filename);
	fp = fopen(filename, "r");
	if (!fp) {
		printf("Could not open for reading: %s\n", filename);
		return -1;
	}

	tokens = SzAlloc(MAX_TOKENS_PER_LINE, jc_token, "Compile_JC");
	if (tokens == null) die("Compile_JC: out of memory");

	while (fgets(line, JC_LINE_LENGTH, fp)) {
		line_no++;
		success = Tokenize_Code_String(line, tokens, &count);

		if (success && count > 0) {
			/* special handling for Include */
			if (tokens[0].type == ttKeyword && streq(tokens[0].value, "Include")) {
				success = Compile_JC(parser, tokens[1].value, false) == 0;
			} else {
				success = Parse_Tokens(parser, tokens, count);
			}
		}

		for (i = 0; i < count; i++) {
			Free(tokens[i].value);
		}

		if (!success) {
			printf("--- jcc died on line %d: %s", line_no, line);
			err = line_no;
			break;
		}
	}
	fclose(fp);

	if (toplevel) Dump_Parser_State(parser);
	Free(tokens);
	return err;
}

bool Dump_Compiled_JC(jc_parser *p, char *filename)
{
	FILE *fp;
	unsigned int i, j, opbytes = 0;

	printf("jcc: dumping progress to %s\n", filename);
	fp = fopen(filename, "w");
	if (!fp) {
		printf("Could not open for writing: %s\n", filename);
		return false;
	}

	for (i = 0; i < p->script_count; i++) {
		fprintf(fp, "\n[%s]\n", p->scripts[i].name);

		for (j = 0; j < p->scripts[i].size; j++) {
			fprintf(fp, "  %04x: %02x", j, p->scripts[i].code[j]);

			switch (p->scripts[i].code[j]) {
				case coPushGlobal:
				case coPushLocal:
				case coPushTemp:
				case coPushInternal:
				case coPopGlobal:
				case coPopLocal:
				case coPopTemp:
					opbytes = 1;
					break;

				case coPushLiteral:
				case coJump:
				case coJumpFalse:
					opbytes = 2;
					break;
			}

			while (opbytes > 0) {
				opbytes--;
				fprintf(fp, " %02x", p->scripts[i].code[++j]);
			}

			fputc('\n', fp);
		}
	}

	fclose(fp);
	return true;
}
