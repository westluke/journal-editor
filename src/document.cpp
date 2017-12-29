#include "document.hpp"

Document::Document(Paragraph b): base(b){
	paragraphs.push_back(base);
}

/*
void Document::append_ch(int ch){
	assert(paragraphs.size() > 0);
	paragraphs[paragraphs.size() - 1].append_ch(ch);
}
*/

/*
void Document::delete_last_ch(){
	assert(paragraphs.size() > 0);
	paragraphs[paragraphs.size() - 1].delete_last_ch();
}
*/

void Document::append_para(){
	assert(paragraphs.size() > 0);
	paragraphs.push_back(base);
}

void Document::insert_ch(DOCLOC docloc, int input){
	paragraphs[docloc.paragraph].insert_ch(docloc.ploc, input);
}

void Document::insert_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].insert_fchar(docloc.ploc, fch);
}

void Document::replace_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].replace_fchar(docloc.ploc, fch);
}

DOCLOC Document::get_DOCLOC(int y, int x){
	for (int i = 0; i < paragraphs.size(); i++){
		if (paragraphs[i].start_line <= y){
			/*
			ploc.pn = y - paras[i].start_line;
			dloc.dn = i;
			dloc.ploc = ploc;
			return dloc;
			*/
			return DOCLOC(i, y - paragraphs[i].start_line, x);
		}
	}

	/*
	ploc.pn = y - paras[paras.size() - 1].start_line;
	dloc.dn = paras.size() - 1;
	dloc.ploc = ploc;
	return dloc;
	*/
	return DOCLOC(paragraphs.size() - 1, y - paragraphs[paragraphs.size() - 1].start_line, x);
}


std::vector<Paragraph> Document::get_paragraphs() const{
	return paragraphs;
}
