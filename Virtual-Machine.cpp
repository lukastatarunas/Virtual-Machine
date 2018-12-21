#include <iostream>
#include <fstream>

unsigned char characters[31];
unsigned char registers[1];
unsigned char position;
bool eof;

void binaryFileRead(unsigned char characters[31])
{
	std::ifstream binaryFile("decryptor.bin", std::ios::binary | std::ios::in);

	binaryFile >> std::noskipws;

	for (auto i = 0; i < 31; i++)
		binaryFile >> characters[i];
}

int textFileRead(unsigned char characters[31], unsigned char registers[1], unsigned char position, bool eof)
{
	std::ifstream textFile("q1_encr.txt");

	while (true)
	{
		unsigned char VMcommand = characters[position];
		unsigned char Rx = (characters[position + 1] & 0b00001111);
		unsigned char Ry = (characters[position + 1] & 0b11110000) >> 4;

		switch (VMcommand)
		{
		case 0x04: registers[0] = characters[position + 1]; // MOVC <<byte constant>>
			break;

		case 0x05: registers[Rx] <<= 1; // LSL Rx
			break;

		case 0x06: registers[Rx] >>= 1; // LSR Rx
			break;

		case 0x07: position = position + 26; // JMP
			break;

		case 0x0A: // JFE
		{
			if (eof == true)
				position = position + 26;
			else
				position = position + 2;
			break;
		}

		case 0x0B: return 0; // RET

		case 0x0C: registers[Rx] = registers[Rx] + registers[Ry]; // ADD Rx, Ry
			break;

		case 0x0D: registers[Rx] = registers[Rx] - registers[Ry]; // SUB Rx, Ry
			break;

		case 0x0E: registers[Rx] = registers[Rx] ^ registers[Ry]; // XOR Rx, Ry
			break;

		case 0x0F: registers[Rx] = registers[Rx] | registers[Ry]; // OR Rx, Ry
			break;

		case 0x10: textFile >> registers[Rx]; // In Rx
			eof = textFile.eof();
			break;

		case 0x11: std::cout << registers[Rx]; // OUT Rx
			break;
		}

		if (VMcommand == 0x07 || VMcommand == 0x0A)
			position = position;
		else
			position = position + 2;
	}
}

int main()
{
	binaryFileRead(characters);
	textFileRead(characters, registers, position, eof);
}
