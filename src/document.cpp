#include "document.hpp"

Document::Document(Paragraph b): base(b){
	paras.push_back(base);
}

void Document::append_ch(int ch){
	assert(paras.size() > 0);
	paras[paras.size() - 1].append_ch(ch);
}

void Document::delete_last_ch(){
	assert(paras.size() > 0);
	paras[paras.size() - 1].delete_last_ch();
}

void Document::append_para(){
	assert(paras.size() > 0);
	paras.push_back(base);
}

void Document::insert_ch(DOCLOC docloc, int input){
	paras[docloc.dn].insert_ch(docloc.ploc, input);
}

DOCLOC get_DOCLOC(int y, int x){
	for (int i = 0; i < paras.size(); i++){
		if (paras[i]
	}
}

/*void get_PLOC(WINDOW* win, std::vector<Paragraph> paras){
	int y, x;
	getyx(win, y, x);

	for (int i = 0; i < paras.size(); i++){
		if (paras[i].line_no >=){
		}
	}

}
