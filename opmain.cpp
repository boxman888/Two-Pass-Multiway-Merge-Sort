#include<iostream>
#include<vector>
#include<map>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdio>

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

	static bool compare_dept(Dept* ls, Dept* rs) {
		return ls->get_managerid() < rs->get_managerid();
	}

	static bool compare_emp(Emp* ls, Emp* rs) {
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

			std::ofstream fout(name.c_str(), std::ios::app);
			//fout.open(name.c_str(), std::ios::out);
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

			std::ofstream fout(name.c_str(), std::ios::app);
			//fout.open(name.c_str(), std::ios::out);
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
			for (size_t i = 0; i < emp.size(); i++)
				delete emp[i];
		}
		if (dept.size() != 0) {
			for (size_t i = 0; i < dept.size(); i++)
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
		fin >> std::noskipws;

		double dd = 0;
		unsigned int count, block_count;
		std::vector<std::string> row;
		std::string line, word, temp, filename;

		File* page = NULL;
		count = block_count = 0;
		while (std::getline(fin, temp)) {
			row.clear();
			std::stringstream ss(temp);

			while (getline(ss, word, ',')) {
				row.push_back(word);
			}
			std::stringstream s;
			if (page == NULL) {
				count++;
				filename.clear();
				s.str(std::string());
				s.clear();
				s << count;
				std::string index = s.str();

				if (type.compare("dept") == 0)
					filename = "tempdept_" + index;

				if (type.compare("emp") == 0)
					filename = "tempemp_" + index;

				page = new File(filename);
			}

			if (type.compare("dept") == 0) {
				std::stringstream sd(row[2]);
				sd >> dd;
				page->build_dept(atoi(row[0].c_str()), atoi(row[3].c_str()), dd, row[1]);
			}
			if (type.compare("emp") == 0) {
				std::stringstream sd(row[3]);
				sd >> dd;
				page->build_emp(atoi(row[0].c_str()), atoi(row[2].c_str()), dd, row[1]);
			}
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

class JoinTables {
private:
	std::fstream emp_file;
	std::fstream dept_file;
	std::fstream out_file;

	std::string empfilename;
	std::string deptfilename;

	std::multimap<int, std::string> page;


	int emp_total;
	int dept_total;
	int block_size;

	void readline(std::vector<std::string>& row, std::fstream& name, std::string& temp) {
		std::string line, word, filename;

		std::stringstream ss(temp);

		while (getline(ss, word, ',')) {
			row.push_back(word);
		}
	}

	std::string buildRecord(std::vector<std::string>& dept_row, std::vector<std::string>& emp_row) {
		std::string r = emp_row[0] + "," + emp_row[1] + "," + emp_row[2] + "," + emp_row[3] + "," + dept_row[1] + "," + dept_row[2] + "," + dept_row[3] + "\n";
		return r;
	}

    void save(){
        std::ofstream fout("join.csv", std::ios::app);
	    std::multimap<int, std::string>::iterator it;
		for (it = page.begin(); it != page.end(); ++it)
			fout << it->second;

        page.clear();
		fout.close();
    }

public:
	JoinTables(std::string empfilename, int emp_total, std::string deptfilename, int dept_total, int block_size) {
		this->empfilename = empfilename;
		this->deptfilename = deptfilename;
		this->emp_total = emp_total;
		this->dept_total = dept_total;
		this->block_size = block_size;
	}

	bool removefile(std::string &file_name){
		char buffer[file_name.length()];
		strcpy(buffer, file_name.c_str());
		if (remove(buffer) != 0)
			return true;
		return false;
	}

    void cleanup(){
        std::stringstream ss;
        std::string id;
        int i;

        for (i= 0; i < dept_total; i++){
            ss << i + 1;
            ss >> id;
            id = deptfilename + id;

            removefile(id);
            ss.clear();
            ss.str(std::string());
        }
        for (i = 0; i < emp_total; i++){
            ss << i + 1;
            ss >> id;
            id = empfilename + id;

            removefile(id);
            ss.clear();
            ss.str(std::string());
        }
    }

	void join() {
        int count, flag;
		std::stringstream ss;
		std::string name, record;
		std::string filename, emp_temp, dept_temp;

		std::vector<std::string> emp_row;
		std::vector<std::string> dept_row;

        count = 0;
        flag = 1;
		for (int i = 0; i < dept_total; i++) {
			ss << i + 1;
			ss >> name;
			dept_file.open((deptfilename + name).c_str(), std::ios::in);

			while (dept_file.is_open()) {
				emp_row.clear();
				dept_row.clear();
				std::getline(dept_file, dept_temp);
				readline(dept_row, dept_file, dept_temp);

				if (dept_file.eof()) {
					dept_file.close();
					continue;
				}

				for (int j = 0; j < emp_total; j++) {
					ss.clear();
					ss.str(std::string());
					ss << j + 1;
					ss >> name;

					emp_file.open((empfilename + name).c_str(), std::ios::in);
					while (emp_file.is_open() && dept_file.is_open() && flag) {
						std::getline(emp_file, emp_temp);
						readline(emp_row, emp_file, emp_temp);

						if (emp_file.eof()) {
							emp_file.close();
							emp_row.clear();
							continue;
						}

						if (dept_row[3].compare(emp_row[0]) == 0){
							emp_file.close();
							record = buildRecord(dept_row, emp_row);

							page.insert(std::pair<int,std::string>((int)atoi(emp_row[0].c_str()), record));
                            flag = 0;
                            count++;
							emp_row.clear();
						}

						emp_row.clear();
					}
                    flag = 1;

                    if (count == block_size)
                        save();
                }
			}
			ss.clear();
			ss.str(std::string());

			dept_file.close();
		}
        if (page.size() > 0)
        	save();
	}
};

int main()
{
	int memory_size = 22;
	std::string sorted_emp;
	std::string sorted_dept;

	FileHandler dept_file("Dept.csv", "dept", (int)floor(memory_size/2));
	FileHandler emp_file("Emp.csv", "emp", (int)floor(memory_size/2));

	int total_dept = dept_file.read_table();
	int total_emp = emp_file.read_table();

	JoinTables table("tempemp_", total_emp, "tempdept_", total_dept, memory_size);
	table.join();
    table.cleanup();

	return 0;
}
