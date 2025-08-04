#include <Arduino.h>
#include <string>
#include "FT891_CAT.h"
#include "Cat.h"

Cat CatInterface;

void Cat::begin()
{
	cat_message.begin(false, &cat_comm, false);
	f_rxtx = false;
	frequency_a = 0;
	lastCat = 0;

	CatInterface.Getag();
	CatInterface.Getrg();
	CatInterface.Getig();
	CatInterface.Requestinformation(2);
}

bool Cat::checkCAT()
{
	bool ret_val = cat_message.CheckCAT();
	return ret_val;
}

// Send FT command to sdrberry
void Cat::Setft(int ft)
{
	cat_message.SetFT(ft);
}

void Cat::Setag(int ag)
{
	cat_message.SetAG((uint8_t)ag);
}

uint8_t Cat::Getag()
{
	return cat_message.GetAG();
}

long long Cat::Getfa()
{
	frequency_a = cat_message.GetFA();
	return frequency_a;
}

void Cat::Setrg(int rg)
{
	cat_message.SetRG((uint8_t)rg);
}

void Cat::Setig(int ig)
{
	cat_message.SetIG((uint8_t)ig);
}

void Cat::Settx(int tx)
{
	cat_message.SetTX((uint8_t)tx);
}

uint8_t Cat::Getrg()
{
	return cat_message.GetRG();
}

uint8_t Cat::Getig()
{
	return cat_message.GetIG();
}

void Cat::Requestinformation(int info)
{
	char str[20];

	sprintf(str, "GT%d;", info);
	cat_comm.Send(str);
}

void Comm::Send(std::string s)
{
	Serial.print(s.c_str());
	Serial.flush();
	char str[BUF_LEN];
}

void Comm::Read(char c, std::string& s)
{
	char szBuffer[BUF_LEN];
	
	memset(szBuffer, 0, BUF_LEN);
	Serial.readBytesUntil(c, szBuffer, sizeof(szBuffer));
	for (int i = 0; i < strlen(szBuffer); i++)
		s.push_back(szBuffer[i]);
}

bool Comm::available()
{
	if (Serial.available())
		return true;
	else
		return false;
}

void Comm::SendInformation(int info)
{

}

void Comm::setband(int band)
{
	//gui_band.set_button(band);
}

void Comm::Processinformation(std::string s)
{
	/*char c = s.at(0);
	switch (c)
	{
	case '0':
		break;
	case '1':
		break;
	case '2':
		break;
	}*/
}

void Cat::SetBnd(int bnd)
{
	cat_message.SetBand(bnd);
}

void Cat::SetIf(int if_filter)
{
	cat_message.SetSH(0,if_filter);
}

int32_t Cat::GetIf()
{
	return cat_message.GetSH();
}

int Cat::GetSM()
{
	return cat_message.GetSM();
}

void Cat::PollSM()
{
	return cat_message.PollSM();
}

int Cat::GetTX()
{
	return cat_message.GetTX();
}
