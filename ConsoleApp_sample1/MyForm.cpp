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
		MessageBox::Show("無効な入力");
	}
	else {
		MessageBox::Show("有効な入力");
	}
}

System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	//テキストファイルの読み込み
	StreamReader^ sr = gcnew StreamReader("C:/Users/chach/Desktop/reading.txt");
	//1行ごとに読み込みを行う
	String^ line;
	//格納先のリスト
	System::Collections::Generic::List<String^>^ readingList = gcnew System::Collections::Generic::List<String^>;
	while ((line = sr->ReadLine()) != nullptr) {
		//ダブルクォーターだと文字列の型がcharになってしまうので、シングルクォーターにする
		for each (String^ v in line->Split(','))
		{
			readingList->Add(v);
		}
	}
}
//MyFormからの継承メソッドにしないと、UIのパーツが使えない
System::Void MyForm::MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	//サンプルデータの準備
	System::Collections::Generic::List<String^>^ sampleList = gcnew System::Collections::Generic::List<String^>;
	//リストの個数
	int listNum = 9;
	//宣言をループ外に出さないと、同じ数字になってしまう
	Random^ rnd = gcnew Random();
	for (int i = 0; i < listNum; i++) {
		//100までの乱数生成
		int r=rnd->Next(1,100);
		sampleList->Add(i + r.ToString());
	}
	//現在時刻の取得
	time_t time_ = time(NULL);
	struct tm local;
	localtime_s(&local, &time_);
	char now[128];
	strftime(now, sizeof(now), "%Y%m%d%H%M%S", &local);
	String^ date_ = gcnew String(now);

	//ファイルの作成,更新日時を取得
	struct _stat buf;
	_stat("C:/Users/chach/Desktop/reading.txt", &buf);
	char str[26];//日時文字列をおいておく変数
	//ctime_s(str, sizeof(str), &buf.st_ctime);//作成日時を指定
	ctime_s(str, sizeof(str), &buf.st_mtime);//更新日時を指定
	//char文字列をSystem::Stringに変換
	String^ update = gcnew String(str);
	//データの書き込み
	StreamWriter^ sw;
	String^ path = "C:/Users/chach/Desktop/reading.txt";
	int count = 1;
	String^ line = "----------";
	try {
		//第二引数のtrueは上書きせずに追加記入する。falseなら上書き
		sw = gcnew StreamWriter(path, true);
		sw->WriteLine(line);
		for each (String^ val in sampleList)
		{
			int res;
			//valが数字なら処理する
			if (int::TryParse(val, res)) {
				//一つの数字ごとに改行
				//sw->WriteLine(val);
				//全数字を一行にまとめる
				/*if (count != sampleList->Count) {
					sw->Write(val + ",");
				}
				else {
					sw->Write(val);
				}*/
				//数字を三つずつで改行
				if (count % 3 != 0) {
					sw->Write(val + ",");
				}
				else {
					sw->WriteLine(val + ",");
				}
				count++;
			}
			else {
				MessageBox::Show("解読不能の値");
			}
		}
		sw->WriteLine(line);
		sw->Write("--更新日時--"+update);
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