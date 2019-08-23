
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define LEN 100
#define FALSE 0
#define TRUE !FALSE
#define CLEAN 0
#define INFECTED 1
#define FIRST20 2
#define LAST20 3
#define LOG "\\AntiVirusLog.txt"
#define NORMALSCAN '0'
#define CANTSCAN -1

extern int errno;

void ignorDots(DIR *dir);
void getString(char str[], int n);
int normalScan(char folderPath[], char file[], FILE * virusFile);

int main(void)
{
	DIR *dir;
	struct dirent *ent;
	FILE * virusFile, *logFile;
	char choice = NORMALSCAN, files[LEN][LEN] = { NULL }, folderPath[LEN] = "C:\\Users\\king\\Desktop\\ldior", virusPath[LEN] = "C:\\Users\\king\\Desktop\\virus\\kaki.txt", logPath[LEN] = { NULL };
	int i = 0, numberOfFiles = 0;


	printf("Welcome to my Virus Scan!\n");
	/*
	printf("\nFolder to scan: ");
	getString(folderPath, LEN);
	printf("Virus signature: ");
	getString(virusPath, LEN);
	*/

	// opening log file
	strcat(logPath, folderPath);
	strcat(logPath, LOG);
	logFile = fopen(logPath, "a");
	fprintf(logFile, "Anti-virus bagan! Welcome!\n\n");
	fprintf(logFile, "Folder to scan:\n%s\n", folderPath);
	fprintf(logFile, "Virus signature:\n%s\n", virusPath);

	// if can open folder
	if ((dir = opendir(folderPath)) != NULL)
	{
		// if can open virus file
		if (virusFile = fopen(virusPath, "rb") != NULL)
		{
			// clears the two first file names because they r dots
			ignorDots(dir);

			// save all the file names and directories within directory 
			while ((ent = readdir(dir)) != NULL)
			{
				strncpy(files[numberOfFiles], ent->d_name, strlen(ent->d_name));
				numberOfFiles++;
			}
			closedir(dir);

			// asks for normal scan or quick scan
			printf("\nPress 0 for normal scan or any other key for a quick scan: ");
			scanf("%c", &choice);
			getchar();

			// normal scan
			if (choice == NORMALSCAN)
			{
				fprintf(logFile, "Scanning option:\nNormal Scan\nResults:\n");
				printf("Scanning began...\nThis process may take several minutes...\n\nScanning:\n");
				for (i = 0; i < numberOfFiles; i++)
				{
					//fputs(folderPath, stdout);
					printf("%s\\%s - ", folderPath, files[i]);
					switch (normalScan(folderPath, files[i], virusFile))
					{
					case CLEAN:
						printf("clean\n");
						fprintf(logFile, "%s\\%s  clean\n", folderPath, files[i]);
						break;

					case  CANTSCAN:
						printf("Can't scan\n");
						break;

					default:
						printf("Infected!\n");
						fprintf(logFile, "%s\\%s  Infected!\n", folderPath, files[i]);
					}
				}
				printf("Scan Completed.\nSee log path for results: %s", logPath);
			}

			// quick scan
			else
			{
				fprintf(logFile, "Scanning option:\nQuick Scan\nResults:\n");
				printf("Scanning began...\nThis process may take several minutes...\n\nScanning:\n");
				for (i = 0; i < numberOfFiles; i++)
				{
					//fputs(folderPath, stdout);
					printf("%s\\%s - ", folderPath, files[i]);
					switch (normalScan(folderPath, files[i], virusFile))
					{
					case CLEAN:
						printf("clean\n");
						fprintf(logFile, "%s\\%s  clean\n", folderPath, files[i]);
						break;

					case INFECTED:
						printf("Infected!\n");
						fprintf(logFile, "%s\\%s  Infected!\n", folderPath, files[i]);
						break;

					case FIRST20:
						printf("Infected! <first 20%>\n");
						fprintf(logFile, "%s\\%s  Infected! (first 20%)\n", folderPath, files[i]);
						break;

					case LAST20:
						printf("Infected! <last 20%>\n");
						fprintf(logFile, "%s\\%s  Infected! (last 20%)\n", folderPath, files[i]);
						break;

					default:
						printf("Can't scan\n");
					}
				}
				printf("Scan Completed.\nSee log path for results: %s", logPath);

			}
		}
		else
		{
			/* could not open file */
			printf("cant open file\n%s", strerror(errno));
		}
	}
	else
	{
		/* could not open directory */
		printf("cant open directory");
	}
	getchar();
	getchar();

	return 0;
}

void ignorDots(DIR *dir)
{
	readdir(dir);
	readdir(dir);
}


void getString(char str[], int n)
{
	fgets(str, n, stdin);
	str[strcspn(str, "\n")] = NULL;
}

int normalScan(char folderPath[], char file[], FILE * virusFile)
{
	char scannedFilePath[LEN] = { NULL }, virusByte = NULL, scannedByte = NULL;
	int result = CLEAN, virusSize = 0, scannedFileSize = 0, i = 0;
	FILE * scannedFile = fopen(strcat(strcat(strcat(scannedFilePath, folderPath), "\\"), file), "rb");




	if (scannedFile != NULL && !strstr(file, "AntiVirusLog.txt"))
	{
		// obtain virusfile size:
		fseek(virusFile, 0, SEEK_END);
		virusSize = ftell(virusFile);
		rewind(virusFile);

		// obtain scannedfile size:
		fseek(scannedFile, 0, SEEK_END);
		scannedFileSize = ftell(scannedFile);
		rewind(scannedFile);

		while (((scannedByte = fgetc(scannedFile)) != EOF) && ((virusByte = fgetc(virusFile)) != EOF))
		{
			if (scannedByte == virusByte)
			{
				result = INFECTED;
			}
			else
			{
				fseek(virusFile, 0, SEEK_SET);
				result = CLEAN;
				break;
			}
		}
	}
	else
	{
		result = CANTSCAN;
	}
	fclose(scannedFile);
	return result;
}