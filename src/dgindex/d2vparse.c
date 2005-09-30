// ParseD2V by Donald A. Graft
// Released under the Gnu Public Licence (GPL)

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "global.h"

int parse_d2v(HWND hWnd, char *szInput)
{
	FILE *fp, *wfp;
	char line[2048], *p;
	int i, fill, val, prev_val = -1, ndx = 0, count = 0, fdom = -1;
	int D2Vformat = 0;
	int vob, cell;
	unsigned int gop_field, field_operation;
	char render[128], temp[20];
	int type;
	
	// Open the D2V file to be parsed.
	fp = fopen(szInput, "r");
	if (fp == 0)
	{
		MessageBox(hWnd, "Cannot open the D2V file!", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	// Mutate the file name to the output text file to receive the parsed data.
	p = &szInput[strlen(szInput)];
	while (*p != '.') p--;
	p[1] = 0;
	strcat(p, "parse.txt");
	// Open the output file.
	wfp = fopen(szInput, "w");
	if (wfp == 0)
	{
		MessageBox(hWnd, "Cannot create the parse output file!", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	fprintf(wfp, "D2V Parse Output\n\n");
	// Pick up the D2V format number
	fgets(line, 2048, fp);
	if (strncmp(line, "DGIndexProjectFile", 18) != 0)
	{
		MessageBox(hWnd, "The file is not a DGIndex project file!", NULL, MB_OK | MB_ICONERROR);
		fclose(fp);
		fclose(wfp);
		return 0;
	}
	sscanf(line, "DGIndexProjectFile%d", &D2Vformat);
	if (D2Vformat != D2V_FILE_VERSION)
	{
		MessageBox(hWnd, "Obsolete D2V file.\nRecreate it with this version of DGIndex.", NULL, MB_OK | MB_ICONERROR);
		fclose(fp);
		fclose(wfp);
		return 0;
	}
	fprintf(wfp, "Encoded Frame: Display Frames....Flags Byte (* means in 3:2 pattern)\n");
	fprintf(wfp, "--------------------------------------------------------------------\n\n");
	// Get the field operation flag.
	while (fgets(line, 2048, fp) != 0)
	{
		if (strncmp(line, "Field_Operation", 8) == 0) break;
	}
	sscanf(line, "Field_Operation=%d", &field_operation);
	if (field_operation == 1)
		fprintf(wfp, "[Forced Film, ignoring flags]\n");
	else if (field_operation == 2)
		fprintf(wfp, "[Raw Frames, ignoring flags]\n");
	else
		fprintf(wfp, "[Field Operation None, using flags]\n");
	// Skip past the rest of the D2V header info to the data lines.
	while (fgets(line, 2048, fp) != 0)
	{
		if (strncmp(line, "Location", 8) == 0) break;
	}
	while (fgets(line, 2048, fp) != 0)
	{
		if (line[0] != '\n') break;
	}
	// Process the data lines.
	do
	{
		// Skip to the TFF/RFF flags entries.
		p = line;
		sscanf(p, "%x", &gop_field);
		if (gop_field & 0x100)
			fprintf(wfp, "[GOP]\n");
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		sscanf(p, "%d", &vob);
		while (*p++ != ' ');
		sscanf(p, "%d", &cell);
		while (*p++ != ' ');
		// Process the flags entries.
		while ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f'))
		{
			sscanf(p, "%x", &val);
			if (val == 0xff)
			{
				fprintf(wfp, "[EOF]\n");
				break;
			}
			// Pick up the frame type.
			type = (val & 0x30) >> 4;
			sprintf(temp, "%d [%s]", ndx, type == 1 ? "I" : (type == 2 ? "P" : "B")); 
			// Isolate the TFF/RFF bits.
			val &= 0x3;
			// Determine field dominance.
			if (fdom == -1)
			{
				fdom = (val >> 1) & 1;
				fprintf(wfp, "[Clip is %s]\n", fdom ? "TFF" : "BFF");
			}

			// Show encoded-to-display mapping.
			if (field_operation == 0)
			{
				if ((count % 2) && (val == 1 || val == 3))
				{
					sprintf(render, "%s: %d,%d,%d", temp, ndx + count/2, ndx + count/2 + 1, ndx + count/2 + 1);
				}
				else if ((count % 2) && !(val == 1 || val == 3))
				{
					sprintf(render, "%s: %d,%d", temp, ndx + count/2, ndx + count/2 + 1);
				}
				else if (!(count % 2) && (val == 1 || val == 3))
				{
					sprintf(render, "%s: %d,%d,%d", temp, ndx + count/2, ndx + count/2, ndx + count/2 + 1);
				}
				else if (!(count % 2) && !(val == 1 || val == 3))
				{
					sprintf(render, "%s: %d,%d", temp, ndx + count/2, ndx + count/2);
				}
				fill = 32 - strlen(render);
				for (i = 0; i < fill; i++) strcat(render, ".");
				sprintf(temp, "%x", val);
				fprintf(wfp, "%s%s", render, temp);
			}
			else
			{
				sprintf(render, "%s: %d,%d", temp, ndx, ndx);
				fill = 32 - strlen(render);
				for (i = 0; i < fill; i++) strcat(render, ".");
				sprintf(temp, "%x", val);
				fprintf(wfp, "%s%s", render, temp);
			}

			// Show vob cell id.
//			printf(" [vob/cell=%d/%d]", vob, cell);

			// Print warnings for 3:2 breaks and field order transitions.
			if ((prev_val >= 0) && ((val == 0 && prev_val == 3) || (val != 0 && val == prev_val + 1)))
			{
					fprintf(wfp, " *");
			}

			if (prev_val >= 0)
			{
				if ((val == 2 && prev_val == 0) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 3 && prev_val == 0) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 0 && prev_val == 1) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 1 && prev_val == 1) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 0 && prev_val == 2) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 1 && prev_val == 2) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 2 && prev_val == 3) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
				else if ((val == 3 && prev_val == 3) || (val == 2 && prev_val == 0))
					fprintf(wfp, " [FIELD ORDER TRANSITION!]");
			}

			fprintf(wfp, "\n");

			// Count the encoded frames.
			ndx++;
			// Count the number of pulled-down fields.
			if (val == 1 || val == 3) count++;
			// Move to the next flags entry.
			while (*p != ' ' && *p != '\n') p++;
			p++;
			prev_val = val;
		}
	} while ((fgets(line, 2048, fp) != 0) &&
			 ((line[0] >= '0' && line[0] <= '9') || (line[0] >= 'a' && line[0] <= 'f')));

	fclose(fp);
	fclose(wfp);
	return 1;
}

int fix_d2v(HWND hWnd, char *Input, int silent)
{
	FILE *fp, *wfp, *dfp;
	char line[2048], prev_line[2048], wfile[2048], logfile[2048], *p, *q;
	int val, mval, prev_val, mprev_val, fix;
	bool first, found;
	int D2Vformat = 0;

	fp = fopen(Input, "r");
	if (fp == 0)
	{
		MessageBox(hWnd, "Cannot open the D2V file!", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	// Pick up the D2V format number
	fgets(line, 1024, fp);
	if (strncmp(line, "DGIndexProjectFile", 18) != 0)
	{
		MessageBox(hWnd, "The file is not a DGIndex project file!", NULL, MB_OK | MB_ICONERROR);
		fclose(fp);
		return 0;
	}
	sscanf(line, "DGIndexProjectFile%d", &D2Vformat);
	if (D2Vformat != D2V_FILE_VERSION)
	{
		MessageBox(hWnd, "Obsolete D2V file.\nRecreate it with this version of DGIndex.", NULL, MB_OK | MB_ICONERROR);
		fclose(fp);
		return 0;
	}

	strcpy(wfile, Input);
	strcat(wfile,".fixed");
	wfp = fopen(wfile, "w");
	if (wfp == 0)
	{
		MessageBox(hWnd, "Cannot create the fixed D2V file!", NULL, MB_OK | MB_ICONERROR);
		fclose(fp);
		return 0;
	}
	fputs(line, wfp);
	// Mutate the file name to the output text file to receive processing status information.
	strcpy(logfile, Input);
	p = &logfile[strlen(logfile)];
	while (*p != '.') p--;
	p[1] = 0;
	strcat(p, "fix.txt");
	// Open the output file.
	dfp = fopen(logfile, "w");
	if (dfp == 0)
	{
		fclose(fp);
		fclose(wfp);
		MessageBox(hWnd, "Cannot create the info output file!", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	fprintf(dfp, "D2V Fix Output\n\n");
	
	while (fgets(line, 1024, fp) != 0)
	{
		fputs(line, wfp);
		if (strncmp(line, "Location", 8) == 0) break;
	}
	fgets(line, 1024, fp);
	fputs(line, wfp);
	fgets(line, 1024, fp);
	prev_line[0] = 0;
	prev_val = -1;
	mprev_val = -1;
	found = false;
	do
	{
		p = line;
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		while (*p++ != ' ');
		first = true;
		while ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f'))
		{
			fix = -1;
			sscanf(p, "%x", &val);
			if (val == 0xff) break;
			// Isolate the TFF/RFF bits.
			mval = val & 0x3;
			if (prev_val >= 0) mprev_val = prev_val & 0x3;
			// Detect field order transitions.
			if (mval == 2 && mprev_val == 0)      fix = 1;
			else if (mval == 3 && mprev_val == 0) fix = 1;
			else if (mval == 0 && mprev_val == 1) fix = 0;
			else if (mval == 1 && mprev_val == 1) fix = 0;
			else if (mval == 0 && mprev_val == 2) fix = 3;
			else if (mval == 1 && mprev_val == 2) fix = 3;
			else if (mval == 2 && mprev_val == 3) fix = 2;
			else if (mval == 3 && mprev_val == 3) fix = 2;
			// Correct the field order transition.
			if (fix >= 0)
			{
				found = true;
				fprintf(dfp, "Field order transition: %x -> %x\n", mprev_val, mval);
				fprintf(dfp, prev_line);
				fprintf(dfp, line);
				if (first == false)
				{
					q = p;
					while (*q-- != ' ');
				}
				else
				{
					q = prev_line;
					while (*q != '\n') q++;
					while (!((*q >= '0' && *q <= '9') || (*q >= 'a' && *q <= 'f'))) q--;
				}
				*q = (char) fix + '0';
				fprintf(dfp, "corrected...\n");
				fprintf(dfp, prev_line);
				fprintf(dfp, line);
				fprintf(dfp, "\n");
			}
			while (*p != ' ' && *p != '\n') p++;
			p++;
			prev_val = val;
			first = false;
		}
		fputs(prev_line, wfp);
		strcpy(prev_line, line);
	} while ((fgets(line, 2048, fp) != 0) &&
			 ((line[0] >= '0' && line[0] <= '9') || (line[0] >= 'a' && line[0] <= 'f')));
	fputs(prev_line, wfp);
	fputs(line, wfp);
	while (fgets(line, 2048, fp) != 0) fputs(line, wfp);
	fclose(fp);
	fclose(wfp);
	if (found == false)
	{
		if (silent)
		{
			fclose(dfp);
			unlink(wfile);
			unlink(logfile);
		}
		else
		{
			fprintf(dfp, "No errors found.\n");
			fclose(dfp);
			unlink(wfile);
			MessageBox(hWnd, "No errors found.", "Fix D2V", MB_OK | MB_ICONINFORMATION);
		}
		return 0;
	}
	else
	{
		FILE *bad, *good, *fixed;
		char c;

		fclose(dfp);
		// Copy the D2V file to *.d2v.bad version.
		good = fopen(Input, "r");
		if (good == 0)
			return 1;
		sprintf(line, "%s.bad", Input);
		bad = fopen(line, "w");
		if (bad == 0)
		{
			fclose(good);
			return 1;
		}
		while ((c = fgetc(good)) != EOF) fputc(c, bad);
		fclose(good);
		fclose(bad);
		// Copy the *.d2v.fixed version to the D2V file.
		good = fopen(Input, "w");
		if (good == 0)
			return 1;
		sprintf(line, "%s.fixed", Input);
		fixed = fopen(line, "r");
		while ((c = fgetc(fixed)) != EOF) fputc(c, good);
		fclose(good);
		fclose(fixed);
		// Ditch the *.d2v.fixed version.
		unlink(line);
		if (!CLIActive)
		{
			MessageBox(hWnd, "D2V file corrected. The errored version was\nsaved with the extension \".bad\".", "Fix D2V", MB_OK | MB_ICONINFORMATION);
			if (!silent)
				ShellExecute(hDlg, "open", logfile, NULL, NULL, SW_SHOWNORMAL);
		}
	}

	return 1;
}

