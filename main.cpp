#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "types.h"
#include "image.h"

using namespace std;

void error(const string& msg) {

	cout << msg << "\n";
	exit(EXIT_FAILURE);

}


template <typename T>
ostream& stream_write(ostream& os, T& val, size_t n = sizeof(T)) {

	return os.write(reinterpret_cast<char*>(&val), n);

}

struct de_tag {

	uint16_t _group_num;
	uint16_t _elem_num;

	de_tag() : _group_num(0), _elem_num(0) {}
	de_tag(uint16_t gn, uint16_t en) : _group_num(gn), _elem_num(en) {}

};
struct data_element_US {

	de_tag _tag;
	string _vr = "US";
	uint16_t _vl_us;
	uint16_t _vf_us;

	data_element_US(uint16_t gn, uint16_t en, uint16_t vf_us) : _tag({ gn, en }), _vl_us(2), _vf_us(vf_us) {}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_us);
		stream_write(os, _vf_us);

		return os;
	}

};

struct data_element_UL {

	de_tag _tag;
	string _vr = "UL";
	uint16_t _vl_ul;
	uint32_t _vf_ul;

	data_element_UL(uint16_t gn, uint16_t en, uint32_t vf_ul) : _tag{ gn, en }, _vl_ul(4), _vf_ul(vf_ul) {}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_ul);
		stream_write(os, _vf_ul);
		return os;
	}
};

struct data_element_CS {

	de_tag _tag;
	string _vr = "CS";
	uint16_t _vl_cs;
	string _vf_cs;

	data_element_CS(uint16_t gn, uint16_t en, uint16_t vl_cs, string vf_cs) : _tag{ gn, en }, _vl_cs(0), _vf_cs(vf_cs) {

		_vl_cs = _vf_cs.size();

		if ((_vl_cs % 2) != 0) {
			_vf_cs.push_back(0x20);
			_vl_cs++;
		}
	}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_cs);
		os << _vf_cs;
		return os;
	}

};

struct data_element_LO {

	de_tag _tag;
	string _vr = "LO";
	uint16_t _vl_lo;
	string _vf_lo;

	data_element_LO(uint16_t gn, uint16_t en, uint16_t vl_lo, string vf_lo) : _tag{ gn, en }, _vl_lo(0), _vf_lo(vf_lo) {

		_vl_lo = _vf_lo.size();

		if ((_vl_lo % 2) != 0) {
			_vf_lo.push_back(0x20);
			_vl_lo++;
		}
	}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_lo);
		os << _vf_lo;
		return os;
	}

};

struct data_element_PN {

	de_tag _tag;
	string _vr = "PN";
	uint16_t _vl_pn;
	string _vf_name, _vf_surname, _vf_pn; //identifica un nome di persona nel formato cognome^nome

	data_element_PN(uint16_t gn, uint16_t en, string vf_name, string vf_surname) : _tag{ gn, en }, _vl_pn(0), _vf_name(vf_name), _vf_surname(vf_surname) {

		_vf_pn = _vf_name + "^" + _vf_surname;

		_vl_pn = _vf_name.size() + _vf_surname.size() + 1;

		if ((_vl_pn % 2) != 0) {
			_vf_pn.push_back(0x20);
			_vl_pn++;
		}

	}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_pn);
		os << _vf_pn;
		return os;
	}

};

struct data_element_UI {

	de_tag _tag;
	string _vr = "UI";
	uint16_t _vl_ui;
	string _vf_ui;

	data_element_UI(uint16_t gn, uint16_t en, uint16_t vl_ui, string vf_ui) : _tag{ gn, en }, _vl_ui(0), _vf_ui(vf_ui) {

		_vl_ui = _vf_ui.size();
		if ((_vl_ui % 2) != 0) {
			_vf_ui.push_back(0x00);
			_vl_ui++;
		}
	}

	ostream& write(ostream& os) {

		stream_write(os, _tag);
		os << _vr;
		stream_write(os, _vl_ui);
		os << _vf_ui;
		return os;
	}
};

struct data_element_OB {

	de_tag _tag;
	string _vr = "OB";
	uint32_t _vl_ob;
	vector<uint8_t> _vf_ob;

	data_element_OB(uint16_t gn, uint16_t en, vector<uint8_t> vf_ob) : _tag{ gn, en }, _vl_ob(0), _vf_ob(vf_ob) {

		_vl_ob = _vf_ob.size();
		if ((_vl_ob % 2) != 0) {

			_vf_ob.push_back(0x00);
			_vl_ob++;
		}
	}

	ostream& write(ostream& os, const size_t& elem_n) {

		stream_write(os, _tag);
		os << _vr;
		os << '\0' << '\0';
		stream_write(os, _vl_ob);
		for (size_t i = 0; i < elem_n; ++i)
			os << _vf_ob[i];
		return os;

	}

};

struct data_element_IMG {

	de_tag _tag;
	string _vr = "OB";
	uint32_t _vl_im;
	char* _vf_im;

	data_element_IMG(uint16_t gn, uint16_t en, uint32_t vl_im, char* vf_im) : _tag{ gn, en }, _vl_im(vl_im), _vf_im(vf_im) {}

	ostream& write(ostream& os, const size_t& elem_n) {

		stream_write(os, _tag);
		os << _vr;
		os << '\0' << '\0';
		stream_write(os, _vl_im);
		for (size_t i = 0; i < elem_n; ++i)
			os << _vf_im[i];
		return os;

	}
};

int main(int argc, char **argv) {

	// Gestione command line

	if (argc != 3)
		error("Errore nel numero dei parametri");

	string input(argv[1]), output(argv[2]);

	// Lettura file txt con informazioni

	ifstream is(input, ios::binary);

	if (!is)
		error("Errore nell'apertura del file!");

	string line;
	string ppm_name, name, surname, id, implementation, sex;


	while (getline(is, line, ':')) {

		if (line == "File") {
			is.ignore(1);
			is >> ppm_name;
		}

		else if (line == "Name") {
			is.ignore(1);
			is >> name;
		}

		else if (line == "Surname") {
			is.ignore(1);
			is >> surname;
		}

		else if (line == "ID") {
			is.ignore(1);
			is >> id;
		}

		else if (line == "Sex") {
			is.ignore(1);
			is >> sex;
		}

		if (line == "Implementation") {
			is.ignore(1);
			is >> implementation;
		}
		getline(is, line);
	}

	// lettura  immagine


	ifstream is2(ppm_name, ios::binary);

	if (!is2)
		error("Errore nell'apertura del file");

	string magic;
	getline(is2, magic);
	if (magic != "P6")
		exit(EXIT_FAILURE);

	// Leggo il commento
	if (is2.peek() == '#') {
		string sComment;
		getline(is2, sComment);
	}

	unsigned w, h, maxval;
	is2 >> w >> h >> maxval;
	if (maxval != 255)
		exit(EXIT_FAILURE);

	is2.get();

	image <vec3b> img(w,h);

	is2.read(img.data(), img.size() * 3);

	//scrittura dati e imamgine in formato DCM

	vector<uint8_t> v(2);
	v[0] = 0;
	v[1] = 1;

	string s0("1.2.840.10008.5.1.4.1.1.77.1.4"), s1("1.2.392.200036.9125.0.19950720112207"), s2("1.2.840.10008.1.2.1"), s3("1.2.392.200036.9125.0.1234567890"),
		s4("ORIGINAL/PRIMARY");

	vector<size_t> len;
	len.push_back(s0.size());
	len.push_back(s1.size());
	len.push_back(s2.size());
	len.push_back(s3.size());

	for (size_t i = 0; i < len.size(); ++i) {
		if ((len[i] % 2) != 0)
			len[i] += 1;
	}

	size_t sum_len = len[0] + len[1] + len[2] + len[3] + 2;

	//gruppo2

	data_element_UL g20(0x0002, 0x0000, 164);
	data_element_OB g21(0x0002, 0x0001, v);
	data_element_UI g22(0x0002, 0x0002, s0.size(), s0);
	data_element_UI g23(0x0002, 0x0003, s1.size(), s1);
	data_element_UI g24(0x0002, 0x0010, s2.size(), s2);
	data_element_UI g25(0x0002, 0x0012, implementation.size(), implementation);

	data_element_CS g80(0x0008, 0x0008, s4.size(), s4);
	data_element_UI g81(0x0008, 0x0016, s0.size(), s0);
	data_element_UI g82(0x0008, 0x0018, s1.size(), s1);


	ofstream os(output, ios::binary);

	for (size_t i = 0; i < 128; ++i)
		os << '\0';

	os << "DICM";


	g20.write(os);
	g21.write(os, v.size());
	g22.write(os);
	g23.write(os);
	g24.write(os);
	g25.write(os);

	g80.write(os);
	g81.write(os);
	g82.write(os);


	data_element_PN gp0(0x0010, 0x0010, name, surname);
	data_element_LO gp1(0x0010, 0x0020, id.size(), id);
	data_element_CS gp2(0x0010, 0x0040, 1, sex);

	gp0.write(os);
	gp1.write(os);
	gp2.write(os);

	data_element_US gd0(0x0028, 0x0002, 3);
	data_element_CS gd1(0x0028, 0x0004, 3, "RGB");
	data_element_US gd2(0x0028, 0x0006, 0);

	data_element_US gd3(0x0028, 0x0010, img.height());
	data_element_US gd4(0x0028, 0x0011, img.width());
	data_element_US gd5(0x0028, 0x0100, 8);
	data_element_US gd6(0x0028, 0x0101, 8);
	data_element_US gd7(0x0028, 0x0102, 7);
	data_element_US gd8(0x0028, 0x0103, 0);
	data_element_CS gd9(0x0028, 0x2110, 2, "00");

	gd0.write(os);
	gd1.write(os);
	gd2.write(os);
	gd3.write(os);
	gd4.write(os);
	gd5.write(os);
	gd6.write(os);
	gd7.write(os);
	gd8.write(os);
	gd9.write(os);


	data_element_IMG gp(0x7FE0, 0x0010, img.size() * 3, img.data());

	gp.write(os, img.size() * 3);


}