#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<sstream>

class Dept {
private:
	int did;
	int managerid;
	double budget;
	std::string dname;

public:
	Dept() {
		did = 0;
		managerid = 0;
		budget = 0.0;
	}

	Dept(int did, int managerid, double budget, std::string dname) {
		set_did(did);
		set_managerid(managerid);
		set_budget(budget);
		set_dname(dname);
	}

	void set_did(int did) {
		this->did = did;
	}

	void set_managerid(int managerid) {
		this->managerid = managerid;
	}

	void set_budget(double budget) {
		this->budget = budget;
	}

	void set_dname(std::string dname) {
		if (dname.size() > (size_t)(floor(40 / sizeof('0'))))
			this->dname = dname.substr(0, (size_t)floor(40 / sizeof('0')));
		else
		this->dname = dname;
	}

	int get_did() {
		return did;
	}

	int get_managerid() {
		return managerid;
	}

	double get_budget() {
		return budget;
	}

	std::string get_dname() {
		return dname;
	}

	/*bool operator<(const Dept& dept) const {
		if (this->managerid < dept.managerid){
			std::cout << "true" << std::endl;
			return true;
		}
		else{
			std::cout << "false" << std::endl;
			return false;
		}
	}*/
};

class Emp {
private:
	int eid;
	int age;
	double salary;
	std::string ename;

public:
	Emp() {
		eid = 0;
		age = 0;
		salary = 0.0;
	}

	Emp(int eid, int age, double salary, std::string ename) {
		set_eid(eid);
		set_age(age);
		set_salary(salary);
		set_ename(ename);
	}

	void set_eid(int eid) {
		this->eid = eid;
	}

	void set_age(int age) {
		this->age = age;
	}

	void set_salary(double salary) {
		this->salary = salary;
	}

	void set_ename(std::string ename) {
		if (ename.size() > (size_t)floor(40 / sizeof('0')))
			this->ename = ename.substr(0, (size_t)floor(40 / sizeof('0')));
		else
		this->ename = ename;
	}

	int get_eid() {
		return eid;
	}

	int get_age() {
		return age;
	}

	double get_salary() {
		return salary;
	}

	std::string get_ename() {
		return ename;
	}
};

class File {
private:
	std::string name;
	std::fstream fout;
	std::vector<Emp*> emp;
	std::vector<Dept*> dept;

	 static bool compare_dept(Dept *ls, Dept *rs) {
		 return ls->get_managerid() < rs->get_managerid();
	}

	static bool compare_emp(Emp *ls, Emp *rs){
		return ls->get_eid() < rs->get_eid();
	}

public:
	File(std::string name) {
		this->name.assign(name);
	}
	void set_file_name(std::string name) {
		this->name.assign(name);
	}

	void build_dept(int did, int managerid, double budget, std::string dname) {
		Dept* dept = new Dept(did, managerid, budget, dname);
		this->dept.push_back(dept);
	}
	void build_emp(int eid, int age, double salary, std::string ename) {
		Emp* emp = new Emp(eid, age, salary, ename);
		this->emp.push_back(emp);
	}

	bool save() {
		if (dept.size() != 0) {
			std::sort(dept.begin(), dept.end(), compare_dept);

			fout.open(name.c_str(), std::ios::out);
			for (size_t i = 0; i < dept.size(); i++) {
				fout << dept[i]->get_did() << ","
					<< dept[i]->get_dname() << ","
					<< dept[i]->get_budget() << ","
					<< dept[i]->get_managerid() << "\n";
			}
			fout.close();
			return true;
		}
		if (emp.size() != 0) {
			std::sort(emp.begin(), emp.end(), compare_emp);

			fout.open(name.c_str(), std::ios::out);
			for (size_t i = 0; i < emp.size(); i++) {
				fout << emp[i]->get_eid() << ","
					<< emp[i]->get_ename() << ","
					<< emp[i]->get_age() << ","
					<< emp[i]->get_salary() << "\n";
			}
			fout.close();
			return true;
		}
		return false;
	}

	std::string get_file_name() {
		return name;
	}

	~File() {
		if (emp.size() != 0) {
			for (size_t i = 0; i < emp.size()-1; i++)
				delete emp[i];
		}
		if (dept.size() != 0) {
			for (size_t i = 0; i < dept.size()-1; i++)
				delete dept[i];
		}
	}
};

class FileHandler {
private:
	unsigned int block_size;
	std::fstream fin;
	std::string table;
	std::string type;

	int read() {
		fin.open(table.c_str(), std::ios::in);

		unsigned int count, block_count;
		std::vector<std::string> row;
		std::string line, word, temp, filename;

		File* page = NULL;
		count = block_count = 0;
		while (fin >> temp) {
			row.clear();
			getline(fin, line);
			std::stringstream ss(temp);

			while (getline(ss, word, ',')) {
				row.push_back(word);
			}

			std::stringstream sd(row[2]);
			double dd = 0.0;
			sd >> dd;

			if (page == NULL) {
				count++;
				filename.clear();
				sd.str(std::string());
				sd.clear();
				sd << count;
				std::string index = sd.str();
				if (type.compare("dept") == 0)
					filename = "tempdept_" + index;
				if (type.compare("emp") == 0)
					filename = "tempemp_" + index;
				page = new File(filename);
			}

			if (type.compare("dept") == 0)
				page->build_dept(atoi(row[0].c_str()), atoi(row[1].c_str()), dd, row[3]);

			if (type.compare("emp") == 0)
				page->build_emp(atoi(row[0].c_str()), atoi(row[1].c_str()), dd, row[3]);

			if (block_count == block_size - 1) {
				page->save();
				block_count = 0;
				delete page;
				page = NULL;
			}
			block_count++;
		}
		if (page != NULL) {
			page->save();
			delete page;
			page = NULL;
		}
		return count;
	}
public:
	FileHandler(std::string table, std::string type, int block_size) {
		if (type.compare("dept") == 0 || type.compare("emp") == 0) {
			this->table.assign(table);
			this->type.assign(type);
			this->block_size = block_size;
		}
		else
			std::cout << "Bad type " << std::endl;
	}

	int read_table() {
		return read();
	}
};

int main()
{
	FileHandler dept_file("Emp.csv", "emp", 22);
	int total = dept_file.read_table();

	std::cout << total << std::endl;

	return 0;
}