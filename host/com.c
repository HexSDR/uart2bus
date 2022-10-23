////////////////////////////////////////////////////////
// RS-232 example
// Compiles with Microsoft Visual C++ 5.0/6.0
// (c) fpga4fun.com KNJN LLC - 2003, 2004, 2005, 2006

#include <windows.h>
#include <stdio.h>
#include <conio.h>

////////////////////////////////////////////////////////
HANDLE hCom;

 
void OpenCom(int com_idx)
{
	DCB dcb;
	COMMTIMEOUTS ct;
	static char com_name [20];
	sprintf(com_name,"COM%d",com_idx); 
	hCom = CreateFile(com_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hCom==INVALID_HANDLE_VALUE) exit(1);
	if(!SetupComm(hCom, 4096, 4096)) exit(1);

	if(!GetCommState(hCom, &dcb)) exit(1);
	dcb.BaudRate = 115200;
	((DWORD*)(&dcb))[2] = 0x1001;  // set port properties for TXDI + no flow-control
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = 2;
	if(!SetCommState(hCom, &dcb)) exit(1);

	// set the timeouts to 0
	ct.ReadIntervalTimeout = MAXDWORD;
	ct.ReadTotalTimeoutMultiplier = 0;
	ct.ReadTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 0;
	ct.WriteTotalTimeoutConstant = 0;
	if(!SetCommTimeouts(hCom, &ct)) exit(1);
}

void CloseCom()
{
	CloseHandle(hCom);
}

DWORD WriteCom(char* buf, int len)
{
	DWORD nSend;
	if(!WriteFile(hCom, buf, len, &nSend, NULL)) exit(1);

	return nSend;
} 

void WriteComChar(char b)
{
	WriteCom(&b, 1);
}

int ReadCom(char *buf, int len)
{
	DWORD nRec;
	if(!ReadFile(hCom, buf, len, &nRec, NULL)) exit(1);
	return (int)nRec;
}

char ReadComChar()
{
	DWORD nRec;
	char c;
	if(!ReadFile(hCom, &c, 1, &nRec, NULL)) exit(1);
	return nRec ? c : 0;
}

void com_putchar(char b)
{
	WriteCom(&b, 1);
}

char  com_getchar (){
	DWORD nRec;
	static char c;
while(1)
{
	if(!ReadFile(hCom, &c, 1, &nRec, NULL)) exit(1);
	if (nRec) return c ;
}
	//return nRec ? c : 0;
}
////////////////////////////////////////////////////////


int uart2bus_wr_reg(unsigned short addr ,unsigned char d ){
	static char buff[10];
	sprintf(buff,"%04X",addr);
	com_putchar('W');
	com_putchar(' ');
	com_putchar( d ) ;
	com_putchar(' ');
	com_putchar(buff[0]);
	com_putchar(buff[1]);
	com_putchar(buff[2]);
	com_putchar(buff[3]);
	com_putchar('\n') ;
}

static unsigned char hex2bin (unsigned char h){
	unsigned char r ,i;
	const char hex_table = {"0123456789abcdef"};
	const char HEX_table = {"0123456789ABCDEF"};
	for (i=0;i<16;++i){if (HEX_table[i]==h) return i ;}
	for (i=10;i<16;++i){if (hex_table[i]==h) return i ;}
}

unsigned char  uart2bus_rd_reg(unsigned short addr){
	static char r,t[2], buff[10];
	sprintf(buff,"%04X",addr);
	com_putchar('R');
	com_putchar(' ');
	com_putchar(buff[0]);
	com_putchar(buff[1]);
	com_putchar(buff[2]);
	com_putchar(buff[3]);
	com_putchar('\n') ;
	r  = com_getchar () ;
	t[0] = hex2bin (r) ; 
	r  = com_getchar () ;
	t[1] = hex2bin (r) ;
	r = t[0] | (t[1]>>4)  ;
	return r  ;
}



void main()
{
	char c, s[256];
	int len;

	OpenCom(1);

	// sending data
//	WriteComChar(0x41);		WriteComChar(0x42);		WriteComChar(0x43);	
//	WriteCom("ABCDE", 5);
	Sleep(3);

	// receiving data
//	len = ReadCom(s, sizeof(s)-1);	s[len] = 0;	printf("%s", s);
//	while(c=ReadComChar()) printf("%c", c);

	CloseCom();
	printf("Press a key to exit");	getch();
}
