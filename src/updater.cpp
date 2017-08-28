#include "Updater.hpp"

Updater::Updater(Printer p): print(p){
	paras.push_back(Paragraph(30));
}

void Updater::insert_ch(int ch){
	paras[0].insert_ch({0, 0}, ch);
	print.print(paras[0].get_lines()[0].get_string(), 0);
}
