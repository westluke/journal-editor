#include "updater.hpp"

Updater::Updater(Printer p): print(p){
	paras.push_back(Paragraph(5));
}

void Updater::insert_ch(int ch){
	paras[0].insert_ch({0, 0}, ch);
	for (int i = 0; i < paras[0].size(); i++){
		print.print(paras[0].get_lines()[i].get_string(), i);
	}
}
