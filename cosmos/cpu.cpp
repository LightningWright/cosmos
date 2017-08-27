#include "cpu.h"


//	OPCODE shortcuts
#define X(opcode)	((opcode & 0x0F00) >> 8)
#define Y(opcode)	((opcode & 0x00F0) >> 4)
#define NNN(opcode)	(opcode & 0x0FFF)
#define NN(opcode)	(opcode & 0x00FF)
#define N(opcode)	(opcode & 0x000F)

RCA1802::RCA1802() {}
RCA1802::~RCA1802() {}

void RCA1802::resetState() {
	pc = 0x200;			// Set program counter to 0x200
	opcode = 0x0;		// Reset op code
	I = 0x0;			// Reset I Special Register
	sp = 0x0;			// Reset Stack Pointer
	
	memset(ram,0, sizeof(ram));			// Reset RAM
	memset(screen,0, sizeof(screen));	// Reset Screen
	memset(stack, 0, sizeof(stack));	// Reset Stack
	memset(keys, 0, sizeof(keys));		// Reset Keys
	memset(V, 0, sizeof(V));			// Reset V Registers
	memcpy(ram , fonts, sizeof(fonts));	// Load Fonts into RAM

	dT = 0x0;			// Reset Delay Timer
	sT = 0x0;			// Reset Sound Timer
	srand(time(NULL));	//Init RNG Seed
}

// Initialize and load ROM into ram
bool RCA1802::loadBinary(const char *file_path) {
	
	cout << "Loading ROM: " << file_path << "\n";

	// Open ROM file ("rb": Read-Binary)
	FILE* rom = fopen(file_path, "rb");

	if (rom == NULL) {
		cerr << "[ERROR] [IO] Failed to Open ROM" << "\n";
		return false;
	}

	// Get file size
	fseek(rom, 0, SEEK_END);
	long rom_size = ftell(rom);
	rewind(rom);

	// Copy buffer to ram
	if ((sizeofRAM - 0x200) > rom_size) {
		fread(ram + 0x200 , rom_size, 1, rom);
	}
	else {
		cerr << "[ERROR] [MEM] ROM size: " << rom_size << " . Exceeds RAM size." << "\n";
		return false;
	}

	fclose(rom);

	return true;
}

void RCA1802::fetch() {
	// Fetch op code
	opcode = ram[pc] << 8 | ram[pc + 1];   // Op code is two bytes
	cout << uppercase << hex;
}

// Execute
void RCA1802::execute() {

	switch (opcode & 0xF000) {

	case 0x0000: {

		switch (opcode & 0x000F) {

		case 0x0000: { // 00E0 - Clear screen
			
			memset(screen, 0, sizeof(screen));	drawFlag = true;
			pc += 2;
			if (debug) { cout << "CLS\n"; }
		}
					 break;

		case 0x000E: // 00EE - Return from subroutine
			--sp;
			pc = stack[sp];
			pc += 2;
			if (debug) { cout << "RET " << setw(6) << pc << "\n"; }
			break;
		}
	}
				 break;

		// 1NNN - Jumps to address NNN
	case 0x1000:
		pc = NNN(opcode);
		if (debug) { cout << "JP  " << setw(6) << pc << "\n"; }
		break;

		// 2NNN - Calls subroutine at NNN
	case 0x2000:
		stack[sp] = pc;
		++sp;
		pc = NNN(opcode);
		if (debug) { cout << "CALL" << setw(6) << pc << "\n"; }
		break;

		// 3XNN - Skips the next instruction if VX equals NN.
	case 0x3000:
		if (V[X(opcode)] == NN(opcode))
			pc += 4;
		else
			pc += 2;
		if (debug) { cout << "SE  " << setw(6) << X(opcode) << "," << setw(4) << NN(opcode) <<  "\n"; }
		break;

		// 4XNN - Skips the next instruction if VX does not equal NN.
	case 0x4000:
		if (V[X(opcode)] != NN(opcode))
			pc += 4;
		else
			pc += 2;
		if (debug) { cout << "SNE " << setw(6) << X(opcode) << "," << setw(4) << X(opcode) << "\n"; }
		break;

		// 5XY0 - Skips the next instruction if VX equals VY.
	case 0x5000:
		if (V[X(opcode)] == V[Y(opcode)])
			pc += 4;
		else
			pc += 2;
		if (debug) { cout << "SE  " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
		break;

		// 6XNN - Sets VX to NN.
	case 0x6000:
		V[X(opcode)] = NN(opcode);
		pc += 2;
		if (debug) { cout << "LD  " << setw(6) << X(opcode) << "," << setw(4) << NN(opcode) << "\n"; }
		break;

		// 7XNN - Adds NN to VX.
	case 0x7000:
		V[X(opcode)] += opcode & 0x00FF;
		pc += 2;
		if (debug) { cout << "ADD " << setw(6) << X(opcode) << "," << setw(4) << NN(opcode) << "\n"; }
		break;

		// 8XY_
	case 0x8000:
		switch (opcode & 0x000F) {

			// 8XY0 - Sets VX to the value of VY.
		case 0x0000:
			V[X(opcode)] = V[Y(opcode)];
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 8XY1 - Sets VX to (VX OR VY).
		case 0x0001:
			V[X(opcode)] |= V[Y(opcode)];
			pc += 2;
			if (debug) { cout << "OR  " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 8XY2 - Sets VX to (VX AND VY).
		case 0x0002:
			V[X(opcode)] &= V[Y(opcode)];
			pc += 2;
			if (debug) { cout << "AND " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 8XY3 - Sets VX to (VX XOR VY).
		case 0x0003:
			V[X(opcode)] ^= V[Y(opcode)];
			pc += 2;
			if (debug) { cout << "XOR " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 8XY4 - Adds VY to VX. VF is set to 1 when there's a carry,
			// and to 0 when there isn't.
		case 0x0004:
			V[X(opcode)] += V[Y(opcode)];
			if (V[Y(opcode)] > (0xFF - V[X(opcode)]))
				V[0xF] = 1; //carry
			else
				V[0xF] = 0;
			pc += 2;
			if (debug) { cout << "ADD " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 8XY5 - VY is subtracted from VX. VF is set to 0 when
			// there's a borrow, and 1 when there isn't.
		case 0x0005:
			if (V[Y(opcode)] > V[X(opcode)])
				V[0xF] = 0; // there is a borrow
			else
				V[0xF] = 1;
			V[X(opcode)] -= V[Y(opcode)];
			pc += 2;
			if (debug) { cout << "SUB " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 0x8XY6 - Shifts VX right by one. VF is set to the value of
			// the least significant bit of VX before the shift.
		case 0x0006:
			V[0xF] = V[X(opcode)] & 0x1;
			V[(opcode & 0x0F00) >> 8] >>= 1;
			pc += 2;
			if (debug) { cout << "SHR " << setw(6) << X(opcode) << "\n"; }
			break;

			// 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's
			// a borrow, and 1 when there isn't.
		case 0x0007:
			if (V[X(opcode)] > V[Y(opcode)])	// VY-VX
				V[0xF] = 0; // there is a borrow
			else
				V[0xF] = 1;
			V[X(opcode)] = V[Y(opcode)] - V[X(opcode)];
			pc += 2;
			if (debug) { cout << "SUBN" << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
			break;

			// 0x8XYE: Shifts VX left by one. VF is set to the value of
			// the most significant bit of VX before the shift.
		case 0x000E:
			V[0xF] = V[X(opcode)] >> 7;
			V[X(opcode)] <<= 1;
			pc += 2;
			if (debug) { cout << "SHL " << setw(6) << X(opcode) << "\n"; }
			break;

		default:
			printf("\nUnknown op code: %.4X\n", opcode);
			exit(3);
		}
		break;

		// 9XY0 - Skips the next instruction if VX doesn't equal VY.
	case 0x9000:
		if (V[X(opcode)] != V[Y(opcode)])
			pc += 4;
		else
			pc += 2;
		if (debug) { cout << "SNE " << setw(6) << X(opcode) << "," << setw(4) << Y(opcode) << "\n"; }
		break;

		// ANNN - Sets I to the address NNN.
	case 0xA000:
		if (debug) { cout << "LD  " << setw(6) << I << "," << setw(4) << NNN(opcode) << "\n"; }
		I = NNN(opcode);
		pc += 2;
		break;

		// BNNN - Jumps to the address NNN plus V0.
	case 0xB000:
		pc = NNN(opcode) + V[0];
		if (debug) { cout << "JP  " << setw(6) << V[0] << "," << setw(4) << NNN(opcode) << "\n"; }
		break;

		// CXNN - Sets VX to a random number, masked by NN.
	case 0xC000:
		V[X(opcode)] = (rand() % (0xFF + 1)) & NN(opcode);
		pc += 2;
		if (debug) { cout << "RND " << setw(6) << X(opcode) << "," << setw(4) << NN(opcode) << "\n"; }
		break;

		// DXYN: Draws a sprite at coordinate (VX, VY) that has a widTh of 8
		// pixels and a height of N pixels.
		// Each row of 8 pixels is read as bit-coded starting from ram
		// location I;
		// I value doesn't change after the execution of this instruction.
		// VF is set to 1 if any screen pixels are flipped from set to unset
		// when the sprite is drawn, and to 0 if that doesn't happen.
	case 0xD000:
	{
		uint16_t op_X = V[X(opcode)];
		uint16_t op_Y = V[Y(opcode)];
		uint16_t op_N = N(opcode);
		uint16_t sprite;
 
		V[0xF] = 0;
		for (int line = 0; line < op_N; line++)
		{
			sprite = ram[I + line];
			for (int offset = 0; offset < 8; offset++)
			{
				if ((sprite & (0x80 >> offset)) != 0) // 0x80 is 0 LSH 8
				{
					if (screen[(op_X + offset + ((op_Y + line) * 64))])	{
						V[0xF] =  1;
					}
					screen[op_X + offset + ((op_Y + line) * 64)] ^= 0x00FFFFFF;
				}
			}
		}

		drawFlag = true;
		pc += 2;
		if (debug) { 
			cout << "DRW " << setw(6) << X(opcode) 
				<< "," << setw(4) << Y(opcode)
				<< "," << setw(4) << N(opcode) << "\n";
		}
	}
	break;

	// EX__
	case 0xE000:

		switch (opcode & 0x00FF) {
			// EX9E - Skips the next instruction if the key stored
			// in VX is pressed.
		case 0x009E:
			if (keys[V[X(opcode)]] != 0){ pc += 4; }
			else { pc += 2; }
			if (debug) { cout << "SKP " << setw(6) << X(opcode) << "\n"; }
			break;

			// EXA1 - Skips the next instruction if the key stored
			// in VX isn't pressed.
		case 0x00A1:
			if (keys[V[X(opcode)]] == 0){ pc += 4; }
			else{ pc += 2; }
			if (debug) { cout << "SKNP" << setw(6) << X(opcode) << "\n"; }
			break;

		default:
			cout << "Unknown " << opcode << "OpCode\n";
			exit(3);
		}
		break;

		// FX__
	case 0xF000:
		switch (opcode & 0x00FF)
		{
			// FX07 - Sets VX to the value of the delay timer
		case 0x0007:
			V[X(opcode)] = dT;
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << X(opcode) << "," << setw(4) << "dT\n"; } //FIX ERROR COUNTDOWN
			break;

			// FX0A - A key press is awaited, and then stored in VX
		case 0x000A:
		{
			bool key_pressed = false;
			int k = 0;

			for (k; k < 16; ++k)
			{
				if (keys[k] != 0)
				{
					V[X(opcode)] = k;
					key_pressed = true;
				}
			}

			if (!key_pressed){ return; } // If no key is pressed, return and try again.
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << X(opcode) << "," << setw(4) << k << "\n"; }
		}
		break;

		// FX15 - Sets the delay timer to VX
		case 0x0015:
			dT = V[X(opcode)];
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << "dT," << setw(4) << X(opcode) << "\n"; }
			break;

			// FX18 - Sets the sound timer to VX
		case 0x0018:
			sT = V[X(opcode)];
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << "sT," << setw(4) << X(opcode) << "\n"; }
			break;

			// FX1E - Adds VX to I
		case 0x001E:
			// VF is set to 1 when range overflow (I+VX>0xFFF), and 0
			// when there isn't.
			if (debug) { cout << "ADD " << setw(6) << I << "," << setw(4) << X(opcode) << "\n"; }

			if (I + V[X(opcode)] > 0xFFF){ V[0xF] = 1; cout << "WARNING OVERFLOW\n"; }
			else {
				V[0xF] = 0;	I += V[X(opcode)];
			}
			pc += 2;
			
			break;

			// FX29 - Sets I to the location of the sprite for the
			// character in VX. Characters 0-F (in hexadecimal) are
			// represented by a 4x5 font
		case 0x0029:
			I = V[X(opcode)] * 0x5;
			pc += 2;
			if (debug) { cout << "LD  " << setw(6) << "F," << setw(4) << X(opcode) << "\n"; }
			break;

			// FX33 - Stores the Binary-coded decimal representation of VX
			// at the addresses I, I plus 1, and I plus 2
		case 0x0033:
			ram[I] = V[X(opcode)] / 100;
			ram[I + 1] = (V[X(opcode)] / 10) % 10;
			ram[I + 2] = (V[X(opcode)] % 100) % 10;

			if (debug) { cout << "LD  " << setw(6) << "B," << setw(4) << X(opcode) << "\n"; }
			pc += 2;
			break;

			// FX55 - Stores V0 to VX in ram starting at address I
		case 0x0055:
			for (int i = 0; i <= X(opcode); ++i)
				ram[I + i] = V[i];

			// On the original interpreter, when the
			// operation is done, I = I + X + 1.
			if (debug) { cout << "LD  " << setw(6) << I << "," << setw(4) << X(opcode) << "\n"; }
			I += X(opcode) + 1;
			pc += 2;
			break;

		case 0x0065:
			for (int i = 0; i <= X(opcode); ++i)
				V[i] = ram[I + i];

			// On the original interpreter,
			// when the operation is done, I = I + X + 1.
			if (debug) { cout << "LD  " << setw(6) << X(opcode) << "," << setw(4) << I << "\n"; }
			I += X(opcode) + 1;
			pc += 2;
			break;

		default:
			cout << "Unknown " << opcode << "OpCode\n";
		}
		break;

	default:
		cout << "Unknown " << opcode << "OpCode\n";
		exit(3);
	}
}
