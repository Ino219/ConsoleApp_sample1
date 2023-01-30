#include "pch.h"
#include "MyForm.h"
#include <chrono>
#include <stdio.h>
#include <time.h>


using namespace System::IO;
using namespace ConsoleAppsample1;
using namespace System::Runtime::InteropServices;

[STAThreadAttribute]

int main() {
	Application::Run(gcnew MyForm());
	return 0;
}

System::Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) {
	if (radioButton2->Checked == true&& !textBox1->Text->Contains("_fix")) {
		MessageBox::Show("�����ȓ���");
	}
	else {
		MessageBox::Show("�L���ȓ���");
	}
}

System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	//�e�L�X�g�t�@�C���̓ǂݍ���
	StreamReader^ sr = gcnew StreamReader("C:/Users/chach/Desktop/reading.txt");
	//1�s���Ƃɓǂݍ��݂��s��
	String^ line;
	//�i�[��̃��X�g
	System::Collections::Generic::List<String^>^ readingList = gcnew System::Collections::Generic::List<String^>;
	while ((line = sr->ReadLine()) != nullptr) {
		//�_�u���N�H�[�^�[���ƕ�����̌^��char�ɂȂ��Ă��܂��̂ŁA�V���O���N�H�[�^�[�ɂ���
		for each (String^ v in line->Split(','))
		{
			readingList->Add(v);
		}
	}
}
//MyForm����̌p�����\�b�h�ɂ��Ȃ��ƁAUI�̃p�[�c���g���Ȃ�
System::Void MyForm::MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	//�T���v���f�[�^�̏���
	System::Collections::Generic::List<String^>^ sampleList = gcnew System::Collections::Generic::List<String^>;
	//���X�g�̌�
	int listNum = 9;
	//�錾�����[�v�O�ɏo���Ȃ��ƁA���������ɂȂ��Ă��܂�
	Random^ rnd = gcnew Random();
	for (int i = 0; i < listNum; i++) {
		//100�܂ł̗�������
		int r=rnd->Next(1,100);
		sampleList->Add(i + r.ToString());
	}
	//���ݎ����̎擾
	time_t time_ = time(NULL);
	struct tm local;
	localtime_s(&local, &time_);
	char now[128];
	strftime(now, sizeof(now), "%Y%m%d%H%M%S", &local);
	String^ date_ = gcnew String(now);

	//�t�@�C���̍쐬,�X�V�������擾
	struct _stat buf;
	_stat("C:/Users/chach/Desktop/reading.txt", &buf);
	char str[26];//����������������Ă����ϐ�
	//ctime_s(str, sizeof(str), &buf.st_ctime);//�쐬�������w��
	ctime_s(str, sizeof(str), &buf.st_mtime);//�X�V�������w��
	//char�������System::String�ɕϊ�
	String^ update = gcnew String(str);
	//�f�[�^�̏�������
	StreamWriter^ sw;
	String^ path = "C:/Users/chach/Desktop/reading.txt";
	int count = 1;
	String^ line = "----------";
	try {
		//��������true�͏㏑�������ɒǉ��L������Bfalse�Ȃ�㏑��
		sw = gcnew StreamWriter(path, true);
		sw->WriteLine(line);
		for each (String^ val in sampleList)
		{
			int res;
			//val�������Ȃ珈������
			if (int::TryParse(val, res)) {
				//��̐������Ƃɉ��s
				//sw->WriteLine(val);
				//�S��������s�ɂ܂Ƃ߂�
				/*if (count != sampleList->Count) {
					sw->Write(val + ",");
				}
				else {
					sw->Write(val);
				}*/
				//�������O���ŉ��s
				if (count % 3 != 0) {
					sw->Write(val + ",");
				}
				else {
					sw->WriteLine(val + ",");
				}
				count++;
			}
			else {
				MessageBox::Show("��Ǖs�\�̒l");
			}
		}
		sw->WriteLine(line);
		sw->Write("--�X�V����--"+update);
		sw->WriteLine(line);
		sw->Write("--TIMESTAMP--"+date_);
	}
	catch (Exception^ e) {
		MessageBox::Show(e->ToString());
	}
	finally{
		sw->Close();
	}
}