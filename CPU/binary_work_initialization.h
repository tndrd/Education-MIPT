
const int SIGNATURE_LENGTH = strlen(SIGNATURE);
const int HEADER_LENGTH = SIGNATURE_LENGTH + 2;

if (FILESIZE - 1 < HEADER_LENGTH){
    printf("Error: file too small to be assembled code");
    return 5;
}

for (int i = 0; i < SIGNATURE_LENGTH; i++){
    if (buffer[i] != SIGNATURE[i]){
        printf("Error: unappropiate signature\n");
        return 4;
    }
} 

int current_version_base = buffer[HEADER_LENGTH - 2] - 48;
int current_version_sub = buffer[HEADER_LENGTH - 1] - 48;

if (current_version_base != VERSION_BASE || current_version_sub != VERSION_SUB){
    printf("Error: code version is %d.%d, should be %d.%d\n", current_version_base, current_version_sub, VERSION_BASE, VERSION_SUB);
    return 3;
}

printf("Signature: ");
for (int i = 0; i < HEADER_LENGTH - 2; i++) printf("%c", buffer[i]);
printf("\nVersion %d.%d\n", buffer[HEADER_LENGTH-2] - 48, buffer[HEADER_LENGTH-1] - 48);
fflush(stdout);

char nlabels = buffer[HEADER_LENGTH];
RIP = HEADER_LENGTH + nlabels*sizeof(int) + 1;