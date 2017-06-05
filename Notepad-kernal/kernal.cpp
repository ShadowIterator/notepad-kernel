#include "kernal.h"

//SICHAR_INFO
inline void SICHAR_INFO::set_fontpc(SIFONT_PC tfontp)
{
	fontpc = tfontp;
}
inline void SICHAR_INFO::set_fontpc(const SIFONT& tfont)
{
	fontpc = &tfont;
}
inline void SICHAR_INFO::set_color(COLORERF tcolor)
{
	color = tcolor;
}
inline void SICHAR_INFO::set_size(CHARSIZE tsize)
{
	size = tsize;
}
inline void SICHAR_INFO::set_cspace(CHARSPACE tcspace)
{
	cspace = tcspace;
}
inline void SICHAR_INFO::set_lspace(LINESPACE tlspace)
{
	lspace = tlspace;
}
inline SIFONT_PC SICHAR_INFO::get_fontpc()
{
	return fontpc;
}
inline COLORERF SICHAR_INFO::get_color()
{
	return color;
}
inline CHARSIZE SICHAR_INFO::get_size()
{
	return size;
}
inline CHARSPACE SICHAR_INFO::get_cspace()
{
	return cspace;
}
inline LINESPACE SICHAR_INFO::get_lspace()
{
	return lspace;
}

//SIDRAW_INFO
inline void SIDRAW_INFO::set_S(const SIRECT& TS)
{
	S = TS;
}
inline void SIDRAW_INFO::set_L(const SIRECT& TL)
{
	S = TL;
}
inline void SIDRAW_INFO::set_POS(const SIPOINT& TPOS)
{
	POS = TPOS;
}
inline const SIRECT& SIDRAW_INFO::get_S()
{
	return S;
}
inline const SIRECT& SIDRAW_INFO::get_L()
{
	return L;
}
inline const SIPOINT& SIDRAW_INFO::get_POS()
{
	return POS;
}
//SICHARNODE
inline void SICHARNODE::set_fontpc(SIFONT_PC tfontp)
{
	char_infop->set_fontpc(tfontp);
}
inline void SICHARNODE::set_fontpc(const SIFONT& tfont)
{
	char_infop->set_fontpc(tfont);
}
inline void SICHARNODE::set_color(COLORERF tcolor)
{
	char_infop->set_color(tcolor);
}
inline void SICHARNODE::set_size(CHARSIZE tsize)
{
	char_infop->set_size(tsize);
}
/*
inline void SICHARNODE::set_char_infop(const SIFONT& tfont, COLORERF tcolor, CHARSIZE tsize)
{
	char_infop->set_fontpc(tfont);
	char_infop->set_color(tcolor);
	char_infop->set_size(tsize);
}
inline void SICHARNODE::set_char_infop(SIFONT_P tfontp, COLORERF tcolor, CHARSIZE tsize);
*/
inline void SICHARNODE::set_draw_infop_S(const SIRECT& TS)
{
	draw_infop->set_S(TS);
}
inline void SICHARNODE::set_draw_infop_L(const SIRECT& TL)
{
	draw_infop->set_L(TL);
}
inline void SICHARNODE::set_draw_infop_P(const SIPOINT& TP)
{
	draw_infop->set_POS(TP);
}
/**
inline void SICHARNODE::set_draw_infop_LP(const SIRECT& TL, const SIPOINT& TP);
inline void SICHARNODE::set_draw_infop(const SIRECT& TS, const SIRECT& TL, const SIPOINT& TP);
*/
inline void SICHARNODE::ins_prev(SICHARNODE* p)
{
	p->prevp = this->prevp;
	p->nextp = this;
	this->prevp->nextp = p;
	this->prevp = p;
}
inline void SICHARNODE::ins_next(SICHARNODE* p)
{
	p->prevp = this;
	p->nextp = this->nextp;
	this->nextp->prevp = p;
	this->nextp = p;
}
inline void SICHARNODE::ins_prev(SICHARNODE* ps, SICHARNODE* pe)
{
	ps->prevp = this->prevp;
	pe->nextp = this;
	this->prevp->nextp = ps;
	this->prevp = pe;
}
inline void SICHARNODE::ins_next(SICHARNODE* ps, SICHARNODE* pe)
{
	ps->prevp = this;
	pe->nextp = this->nextp;
	this->nextp->prevp = pe;
	this->nextp = ps;
}
inline void SICHARNODE::ins_prev(const SIRANGE& range)
{
	ins_prev(range.sp, range.ep);
}
inline void SICHARNODE::ins_next(const SIRANGE& range)
{
	ins_next(range.sp, range.ep);
}
void del(SICHARNODE* p)
{
	p->prevp->nextp = p->nextp;
	p->nextp->prevp = p->prevp;
	delete p;
}
void del(SICHARNODE* ps, SICHARNODE* pe)
{
	if (ps == pe)
	{
		del(ps);
		return;
	}
	ps->prevp->nextp = pe->nextp;
	pe->nextp->prevp = ps->prevp;
	for (SICHARNODE_P p = ps->nextp; p != pe; p = p->nextp) delete p->prevp;
	delete pe;
}
void del(const SIRANGE& range)
{
	del(range.sp, range.ep);
}
///<interface>
inline const SICHAR_INFO_P SICHARNODE::get_char_infop()
{
	return char_infop;
}
inline const SIDRAW_INFO_P SICHARNODE::get_draw_infop()
{
	return draw_infop;
}
///<\interface>

//SITEXT
//const SIDIRECT SITEXT::LEFT = 0;
//const SIDIRECT SITEXT::RRIHT = 1;
//const SIDIRECT SITEXT::UP = 2;
//const SIDIRECT SITEXT::DOWN = 3;
///<private>
inline void SITEXT::draw_line_from_left(SICHARNODE_P ps, SICHARNODE_P pe, int sx, int y, int line_height, int deltax)
{
	int x;
	SICHARNODE_P p;
	SIDRAW_INFO_P pd;
	
	for (x = sx, p = ps; p != pe; p = p->nextp)
	{
		pd = p->draw_infop;
		pd->L.height = line_height;
		pd->L.width = pd->S.width + p->char_infop->cspace + deltax;
		pd->POS = SIPOINT(x, y);
		x += pd->L.width;
	}

}
inline void SITEXT::proc_line(SICHARNODE_P ps, SICHARNODE_P pe,
	int n, int y, int line_height, int tot_width, int align)
{
	int sx = 0, deltax = 0;

	if (align == ANORMAL || align == ALEFT)
	{
		//draw_line_from_left(ps, pe, 0, y, line_height, 0);
		sx = 0;
		deltax = 0;
	}
	if (align == ARIGHT)
	{
		//draw_line_from_left(ps, pe, pagewidth - tot_width, y, line_height, 0);
		sx = pagewidth - tot_width;
		deltax = 0;
	}
	if (align == ACENTER)
	{
		//draw_line_from_left(ps, pe, (pagewidth - tot_width) >> 1, y, line_height, 0);
		sx = (pagewidth - tot_width) >> 1;
		deltax = 0;
	}
	if (align == ADISTRIBUTED)
	{
		//draw_line_from_left(ps, pe, 0, y, line_height, (pagewidth - tot_width) / n);
		sx = 0;
		deltax = (pagewidth - tot_width) / n;
	}

	draw_line_from_left(ps, pe, sx, y, line_height, deltax);
	return;
}
 
inline bool isenter(SICHAR_T ch)
{
	return ch == '\n' || ch == '\r';
}

inline int Max(int a, int b)
{
	return a > b ? a : b;
}

inline void SITEXT::proc_text()
{
	SICHARNODE_P ps, pe, pps;
	SIDRAW_INFO_P pd;
	SICHAR_INFO_P pc;
	int tot_width;
	int n;
	int line_height;
	int y;

	vlinep.clear();
	vparap.clear();

	for (y = 0, ps = headp->nextp, pps = headp; ps != tailp; ps = pe)
	{
		tot_width = 0;
		line_height = 0;
		n = 0;
		for (pe = ps; tot_width <= pagewidth && (!isenter(pe->ch)); pe = pe->nextp)
		{
			++n;
			pd = pe->draw_infop;
			pc = pe->char_infop;
			tot_width += (pd->S.width + pc->cspace);
			line_height = Max(line_height, pd->S.height + pc->lspace);
			if (tot_width > pagewidth) break;
		}
		--n;
		if (isenter(pe->ch) && pe != tailp)
		{
			vparap.push_back(SIPARAGRAPH(pps->nextp, pe));
			pps = pe;
			pe = pe->nextp;
		}
		vlinep.push_back(SILINE(ps, pe));
		proc_line(ps, pe, n, y, line_height, tot_width, ps->char_infop->align);
		y += line_height;
		if (pe == tailp) break;
	}
}

///<\private>
//constructor
SITEXT::SITEXT()
{
	headp = new SICHARNODE('\n');
	tailp = new SICHARNODE('\n');
	headp->nextp = tailp;
	tailp->prevp = headp;
	cursorp = tailp;
	pagewidth = DEFAULT_PAGEWIDTH;
	vlinep.clear();
	vparap.clear();

	text_changed_f = false;
	set_curfontp_f = false;
	inselect = false;
	fwdnum = 0;
}
/*
///<interface>
//Operate method
///inline void load();
///inline void save();
inline void SITEXT::_init();
*/

template<class T>
void exchange(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

inline void SITEXT::ins_char(SICHAR_T tchar)
{
	cursorp->ins_prev(new SICHARNODE(tchar));
}

inline void SITEXT::ins_char(SICHAR_T ch, int twidth,int theight)
{
	ins_char(ch);
	cursorp->prevp->draw_infop->S.width = twidth;
	cursorp->prevp->draw_infop->S.height = theight;
}

inline void SITEXT::del_char(bool backwards = true)
{
	if (backwards&&cursorp->prevp != headp) del(cursorp->prevp);
	if ((!backwards) && cursorp->nextp != NULL)
	{
		cursorp = cursorp->nextp;
		del(cursorp->prevp);
	}
}

inline void SITEXT::start_select()
{
	select.sp = cursorp;
	inselect = true;
	fwdnum = 0;
}

inline void SITEXT::end_select()
{
	select.ep = cursorp;
	if (fwdnum < 0) exchange<SICHARNODE_P>(select.sp, select.ep);
	if (fwdnum == 0) select._clear();
	inselect = false;
	fwdnum = 0;
}

inline void SITEXT::cancel_select()
{
	select._clear();
}

inline void SITEXT::del_select()
{
	del(select.sp, select.ep);
	cancel_select();
}

inline void SITEXT::replace_select(SICHARNODE_P ps, SICHARNODE_P pe)
{
	SICHARNODE_P p = select.ep->nextp;
	del(select);
	p->ins_prev(ps, pe);
}

inline void SITEXT::replace_select(const SIRANGE& range)
{
	replace_select(range.sp, range.ep);
}

/*
inline void SITEXT::replace_select()
*/

inline void SITEXT::mov_select(SICURSORP tcursorp)
{
	select.sp->prevp->nextp = select.ep->nextp;
	select.ep->nextp->prevp = select.sp->prevp;
	tcursorp->ins_prev(select.sp, select.ep);
}


inline void SITEXT::mov_cursorp(SICURSORP tcursorp)
{
	cursorp = tcursorp;
}
inline void SITEXT::mov_cursorp(SIDIRECT tdir)
{
	switch (tdir)
	{
	case DLEFT:
		if (cursorp->prevp != headp) cursorp = cursorp->prevp;
		break;
	case DRIGHT:
		if (cursorp->nextp != NULL) cursorp = cursorp->nextp;
		break;
	case DUP:
		if (cursorp->draw_infop->POS.y != 0)
			cursorp = point_to_cursorp(cursorp->draw_infop->POS - (0, -1));
		break;
	case DDOWN:
		//if(cursorp->draw_infop->POS.y!=MAX_POSY)
		cursorp = point_to_cursorp(cursorp->draw_infop->POS + (0, cursorp->draw_infop->L.width + 1));
		break;
	}

}
inline void SITEXT::mov_cursorp(const SIPOINT& P)
{
	cursorp = point_to_cursorp(P);
}

///several set_* method
inline void SITEXT::set_select_font(SIFONT_PC tfontpc)
{
	for (SICHARNODE_P p = select.sp; p != select.ep; p = p->nextp)
		p->char_infop->fontpc = tfontpc;
	select.ep->char_infop->fontpc = tfontpc;
}
inline void SITEXT::set_select_font(const SIFONT& tfont)
{
	for (SICHARNODE_P p = select.sp; p != select.ep; p = p->nextp)
		p->char_infop->fontpc = &tfont;
	select.ep->char_infop->fontpc = &tfont;
}
inline void SITEXT::set_curfont(SIFONT_PC tcurfontpc)
{
	curfontpc = tcurfontpc;
}
inline void SITEXT::set_curfont(const SIFONT& tcurfont)
{
	curfontpc = &tcurfont;
}
inline void SITEXT::set_select_color(COLORERF tcolor)
{
	for (SICHARNODE_P p = select.sp; p != select.ep; p = p->nextp)
		p->char_infop->color = tcolor;
	select.ep->char_infop->color = tcolor;
}
inline void SITEXT::set_select_lspace(LINESPACE tlspace)
{
	for (SICHARNODE_P p = select.sp; p != select.ep; p = p->nextp)
		p->char_infop->lspace = tlspace;
	select.ep->char_infop->lspace = tlspace;
}
inline void SITEXT::set_select_cspace(CHARSPACE tcspace)
{
	for (SICHARNODE_P p = select.sp; p != select.ep; p = p->nextp)
		p->char_infop->cspace = tcspace;
	select.ep->char_infop->cspace = tcspace;
}
inline void SITEXT::set_pagewidth(PAGEWIDTH tpagewidth)
{
	pagewidth = tpagewidth;
}

///several get_* method
inline bool point_on_line(SILINE L,const SIPOINT& P)
{
	return L.sp->draw_infop->POS.y <= P.y && 
		P.y < L.sp->draw_infop->POS.y+L.sp->draw_infop->L.height;
}
inline bool point_on_char_col(SICHARNODE_P np, const SIPOINT& P)
{
	return np->draw_infop->POS.x <= P.x&&
		P.x < np->draw_infop->POS.x + np->draw_infop->L.width;
}
inline SICURSORP SITEXT::point_to_cursorp(const SIPOINT& P)
{
	SICURSORP p;
	std::vector<SILINE>::iterator it;
	for (it = vlinep.begin(); it != vlinep.end(); ++it)
		if (point_on_line(*it, P)) break;
	for (p = it->sp; p != it->ep; p = p->nextp)
		if (point_on_char_col(p, P)) break;
	if (p == it->ep) p = p->prevp;
	return p;
}

inline void SITEXT::repaint()
{
	proc_text();  
}

void SITEXT::print_list()
{
	for (SICHARNODE_P p = headp->nextp; p != tailp; p = p->nextp)
	{
		if (isenter(p->ch))
		{
			printf("-------------------------\n");
			continue;
		}
		printf("%c\n",p->ch);
//		p->draw_infop->POS->print();
		p->draw_infop->POS.print();
		printf("\n");
		p->draw_infop->L.print();
		printf("\n\n");
	}
}
/*
//Draw method
///cursor - coord transparent
///divide line
///line texing
inline void SITEXT::repaint()
{

}
*/
//#include<ctime>
int main()
{
//	srand(time(0));
//	printf("%d", rand() % 4);
	SITEXT* textp;
	textp = new SITEXT;
	textp->ins_char('a',10,5);
	textp->cursorp->prevp->char_infop->set_cspace(50);
	textp->cursorp->prevp->char_infop->set_lspace(10);
	textp->ins_char('b', 100, 7);
	textp->ins_char('c',13,5);
//	textp->mov_cursorp(SITEXT::DLEFT);
	textp->ins_char('d', 12, 10);
	textp->ins_char('\n', 0, 0);
	textp->ins_char('X', 1, 1);
	textp->cursorp->prevp->char_infop->set_lspace(10);
	textp->ins_char('Z', 12, 20);
	textp->cursorp->prevp->char_infop->set_cspace(88);
	textp->repaint();
	textp->print_list();

	return 0;
}
