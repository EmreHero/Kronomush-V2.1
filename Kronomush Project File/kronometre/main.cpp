#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>


HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

namespace sa {

	int saat = 0, dakika = 0, saniye = 0; // ilk saatin degerleri

	int gecmisSaat = 0, gecmisDakika = 0, gecmisSaniye = 0; // eski surenin degerleri

	int gecenSaat = 0, gecenDakika = 0, gecenSaniye = 0;

	int eski[3], yeni[3];

	bool dur = true, enter = false, oldu = false;

	char input;

	std::string sure, tarih, gecenSure, eskiSure = "", eskiStr, yeniStr;

	std::streampos pos;

	std::fstream txt;
}

void SetWindow(int Width, int Height) {

	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
}

std::string takvim() { 
	
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	sa::tarih = std::to_string(ltm.tm_mday) + '.' + std::to_string(ltm.tm_mon + 1) + '.' + std::to_string(ltm.tm_year + 1900);

	return sa::tarih;
}

std::string gecenSure() {

	if (sa::gecenSaat < 10) {

		sa::gecenSure = "0";
	}
	else {
		sa::gecenSure = "";
	}
	sa::gecenSure += std::to_string(sa::gecenSaat) + ":";
	if (sa::gecenDakika < 10) {

		sa::gecenSure += "0";
	}
	sa::gecenSure += std::to_string(sa::gecenDakika) + ":";
	if (sa::gecenSaniye < 10) {

		sa::gecenSure += "0";
	}
	sa::gecenSure += std::to_string(sa::gecenSaniye);

	return sa::gecenSure;
}

void baslangictasaatkac() {

	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	sa::saniye = ltm.tm_sec;
}

void suregecmismi() {

	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	if (sa::saniye != ltm.tm_sec) {

		sa::gecenSaniye++;

		sa::saniye = ltm.tm_sec;

		if (sa::gecenSaniye == 60) {

			sa::gecenSaniye = 0;
			
			sa::gecenDakika++;

			if (sa::gecenDakika == 60) {

				sa::gecenDakika = 0;

				sa::gecenSaat++;
			}
		}
	}
}

//sayac() rip o7
/*std::string sayac() {

	sa::sure = "";

	if (sa::saat < 10) {
		//std::cout << "0";
		sa::sure += "0";
	}

	//std::cout << saat << ":";
	sa::sure += std::to_string(sa::saat) + ":";

	if (sa::dakika < 10) {

		//std::cout << "0";	
		sa::sure += "0";
	}

	//std::cout << dakika << ":";
	sa::sure += std::to_string(sa::dakika) + ":";

	if (sa::saniye < 10) {

		//std::cout << "0";	
		sa::sure += "0";
	}

	//std::cout << saniye;
	sa::sure += std::to_string(sa::saniye);

	if (!sa::dur) {

		sa::saniye++;
	}
	if (sa::saniye == 60) {
		sa::saniye = 0;
		sa::dakika++;
	}
	if (sa::dakika == 60) {
		sa::dakika = 0;
		sa::saat++;
	}

	if (!sa::enter) {

		return sa::sure;
	}
	else {

		return "";
	}
}*/

std::string saatstrTopla() {

	for (int i = 0; i < 8; i++) {

		if (sa::eskiSure != "") {

			sa::eskiStr = sa::eskiSure.substr(i, 1);
		}
		sa::yeniStr = sa::gecenSure.substr(i, 1);

		//std::cout << "\neski:" << sa::eskiStr << "\nyeni:" << sa::yeniStr << '\n';
		switch (i) {

			case 0: case 3: case 6:
				if (sa::eskiSure != "") {

					sa::eski[i / 3] = 10 * std::stoi(sa::eskiStr);
				}
				sa::yeni[i / 3] = 10 * std::stoi(sa::yeniStr);
				break;

			case 1: case 4: case 7:

				if (sa::eskiSure != "") {

					sa::eski[(i - 1) / 3] += std::stoi(sa::eskiStr);
				}
				sa::yeni[(i - 1) / 3] += std::stoi(sa::yeniStr);
				break;
		}
	}

	sa::yeniStr = "";

	for (int i = 2; i >= 0; i--) {

		sa::yeni[i] += sa::eski[i];

		if (i != 0 && sa::yeni[i] >= 60) {

			sa::yeni[i] -= 60;
			sa::yeni[i - 1] += 1;
		}
	}

	for (int i = 0; i < 3; i++) {

		if (sa::yeni[i] < 10) {

			sa::yeniStr += "0";
		}

		sa::yeniStr += std::to_string(sa::yeni[i]);

		if (i != 2) {

			sa::yeniStr += ":";
		}
	}
	return sa::yeniStr;
}

void inputf() {

	while (true) {

		sa::input = _getch();
		
		Sleep(10);
	}
}

void Update() {
	
	char eh;
	std::stringstream title;

	while (true) {

		// duraklatma
		if (!sa::dur) {

			title.str("[CALISIYOR] " + sa::sure);
			SetConsoleTitleA(title.str().c_str());
			SetConsoleTextAttribute(handle, 5); // mor

			suregecmismi();
		}
		else {

			title.str("Kronomush");
			SetConsoleTitleA(title.str().c_str());
			SetConsoleTextAttribute(handle, 3); // mavi

			baslangictasaatkac();
		}

		// sifirlama
		if (sa::enter) {

			std::cout << "\n\nSifirla? (E/H): ";
			std::cin >> eh;

			if ((eh == 'e') ^ (eh == 'E')) {

				sa::gecenSaat = 0;
				sa::gecenDakika = 0;
				sa::gecenSaniye = 0;

				sa::txt.seekg(sa::pos);
				sa::eskiSure = saatstrTopla();
				sa::txt << '\n' << sa::eskiSure;
			}

			system("cls");

			SetConsoleTextAttribute(handle, 5);
			std::cout << "  Kolay gelsin~\n\n";
			SetConsoleTextAttribute(handle, 3);

			sa::enter = false;
		}

		// input
		if (!sa::enter) {

			switch (sa::input) {

				case ' ':
					sa::input = '\0';
					sa::dur = !sa::dur;
					break;

				case '\r':
					sa::input = '\0';
					sa::dur = true;
					sa::enter = true;
					break;
			}
		}

		Sleep(10);
	}
}

int main() {

	// konsol ayarlari
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 24;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");    // Choose your font
	SetCurrentConsoleFontEx(handle, FALSE, &cfi);
	SetWindow(28, 5);

	// :3
	SetConsoleTextAttribute(handle, 5);
	std::cout << "  Kolay gelsin~\n\n";
	SetConsoleTextAttribute(handle, 3);

	// thread
	std::thread updateth(Update);
	updateth.detach();
	std::thread inputth(inputf);
	inputth.detach();

	// fstream
	sa::txt.open("kayitlar.txt");

	// start
	char bumu;
	std::string kayitlitarih = "";
	int fileLength = 0;

	while (sa::txt >> bumu) {

		fileLength++;

		if (bumu == '[') {

			kayitlitarih = "";

			while (sa::txt >> bumu) {

				if (bumu == ']') {

					sa::eskiSure = "";

					if (kayitlitarih == takvim()) {

						sa::oldu = true;
						sa::pos = sa::txt.tellg();
					}

					kayitlitarih = "";

					break;
				}
				else {

					kayitlitarih += bumu;
				}
			}

			if (sa::oldu) {
				
				for (int i = 0; i < 8; i++) {

					if (sa::txt >> bumu) {

						sa::eskiSure += bumu;
					}
				}

			}
		}
	}

	sa::txt.close();
	sa::txt.open("kayitlar.txt");
	
	if (!sa::oldu) {

		sa::txt.seekg(0, std::ios_base::end);

		if (fileLength != 0) {
			sa::txt << "\n\n";
		}

		
		sa::txt << "[" << takvim() << ']';
		sa::pos = sa::txt.tellg();
	}

	// dongu
	while (sa::txt) {

		if (!sa::enter) {

			std::cout << "\r	  " << gecenSure();

			sa::txt.seekg(sa::pos);
			//sa::txt << '\n' << saatstrTopla();

			Sleep(500);
		}
	}

	// closing section
	if (!sa::txt) {

		std::cout << "HATA:\n kayitlar.txt bulunamadi.\n";
	}

	system("pause");
}