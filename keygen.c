#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

int main(void)
{
 // used for hashBuffer generation
 char currentLetter = 0;

 // used for password character
 char currentLetterPassword = 0;

 unsigned int currentLetterNumber = 0;
 char userName[15] = { 0 };
 char password[15] = { 0 };

 int multiplyBy = 1; // esi 0x402279
 int userNameLength = 0; // bl  0x0040227E
 int index = 0;      // ecx
 unsigned int totalUserName = 0;  // edx
 char hash[15] = { 0 }; // edi
 int hashLength = 0;
 int x;
 char hashBuffer[15] = {0};
 char hashBufferCopy[15] = {0};
 char format[2] = {0};
 int lastChar = 0; // last char in hash

 strcpy(format, "%i");


 /*hashBuffer = (char*) malloc(1 * sizeof(char));*/
 memset(hashBuffer, 0, sizeof(hashBuffer));

 //printf("Enter username: ");
 //scanf("%s", &userName);
 strcpy(userName, "JoeMarc");
 strcpy(password, "12345");

 userNameLength = strlen(userName);

 printf("Username is %d characters long\n", userNameLength);

 while (index != userNameLength)
 {
   currentLetter = userName[index]; // mov al, byte ptr UserName[ecx]
   currentLetterNumber = (int) currentLetter;
   currentLetterNumber *= multiplyBy;   // imul eax, esi
   currentLetterNumber += multiplyBy;   // add eax, esi
   currentLetterNumber <<= 0xBA;
   currentLetterNumber >>= 0xBE;         // shr eax, 4 (0xBE - 0xBA = 4)
   currentLetterNumber ^= multiplyBy;     // xor eax, esi
   currentLetterNumber += 0x32;       // add al, 32h
   hash[index] = (char)currentLetterNumber;      // mov [edi], eax
   totalUserName += (int) currentLetterNumber;  // add edx, eax
   multiplyBy += 1;        // inc esi
   index += 1;            // inc ecx
 }

 __asm
 {
   mov edx, totalUserName
   imul edx, dword ptr hash
   ;mov totalUserName, eax
   not edx
   mov totalUserName, edx
 }

 /*totalUserName = ~totalUserName;  // not edx
 /*
  push edx
  push offset aI ; "%i"
  push offset hashBufer ;LPSTR
  call ds:wsprintfA
 */

 //lastChar = strlen(hash) - 1;
 wsprintfA(hashBuffer, "%i", totalUserName);

 strcpy(hashBufferCopy, hashBuffer);  // mov edi, [ebp+ecx+arg_0]

 /*
  * Create new Hash from HashBuffer
  */

 index = 0;

 while (1)
 {
   currentLetter = hashBufferCopy[index];

   if (currentLetter == '\0')
	 break;

   currentLetterNumber = (int) currentLetter;
   currentLetterNumber ^= 0x0DEADBABE;
   hashBufferCopy[index] = (char) currentLetterNumber;
   index++;
 }

 /*
  * 0x004023E4 Loop
  */

 index = 0;

 while (1)
 {
   /* 0x004023E4 */
   currentLetter = hashBufferCopy[index];

   if (currentLetter == '\0')
	 break;

   currentLetterNumber = (int) currentLetter;

   /* 0x004023EF */
   if (currentLetterNumber == 0x93)
   {
	 hashBufferCopy[index] = (char) '0x88';
   }
   /* 0x004023F6 */
   /* inc ecx */
   index += 1;
 }

 /*
  * 0x004023F9
  * mov edi, 0x3A666Bh
  */

 __asm
 {
   lea eax, hashBufferCopy
   mov eax, 0x3A666B
 }

 /*
  * CheckPassword
  * 0x004022D7
  */

 index = 0;

 while (1)
 {
   /*currentLetterPassword = password[index];*/

	if (password[index] == '\0')
		break;

   currentLetter = hashBufferCopy[index];

   __asm
   {
	   mov dl, currentLetter
	   xor dl, 0xBE
	   mov eax, index
	   mov password[eax], dl
   }

   /*
   currentLetterNumber = (int) currentLetter;
   currentLetterNumber ^= 0xBE;
   currentLetter = (char) currentLetterNumber;
   password[index] = currentLetter;*/

   index++;
 }

   printf("Password: %s\n", password);

//   getch();
}

