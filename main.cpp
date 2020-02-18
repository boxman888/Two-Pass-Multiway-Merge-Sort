#include<iostream>
#include<vector>
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

	static bool compare_dept(Dept *ls, Dept *rs) {
		return ls->get_managerid() < rs->get_managerid();
	}

	static bool compare_emp(Emp *ls, Emp *rs) {
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
			for (size_t i = 0; i < emp.size() - 1; i++)
				delete emp[i];
		}
		if (dept.size() != 0) {
			for (size_t i = 0; i < dept.size() - 1; i++)
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

class BufferHandler {
private:
	std::fstream first_buffer;
	std::fstream second_buffer;
	std::string name;
	std::string type;

	int total;
	int block_size;

	void read(std::vector<std::string> &row, std::fstream &name, std::string &temp) {
		std::string line, word, filename;

		//getline(name, line);
		std::stringstream ss(temp);

		while (getline(ss, word, ',')) {
			row.push_back(word);
		}
	}

	bool file_compare(std::string file_one, std::string file_two) {
		if (atoi(file_one.c_str()) < atoi(file_two.c_str()))
			return true;
		return false;
	}

	void addToPage(File *page, std::vector<std::string> row) {
		double d = 0.0;

		if (type.compare("dept") == 0) {
			std::stringstream ss(row[2]);
			ss >> d;
			page->build_dept(atoi(row[0].c_str()), atoi(row[3].c_str()), d, row[1]);
		}
		if (type.compare("emp") == 0) {
			std::stringstream ss(row[3]);
			ss >> d;
			page->build_emp(atoi(row[0].c_str()), atoi(row[2].c_str()), d, row[1]);
		}
	}

public:
	BufferHandler(std::string name, std::string type, int total, int block_size) {
		this->name = name;
		this->total = total;
		this->block_size = block_size;
		this->type = type;
	}

	std::string fetch_file_name(int i) {
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		return name + index;
	}

	std::string merge(std::string file_one, std::string file_two, int file_id) {
		first_buffer.open(file_one.c_str(), std::ios::in);
		second_buffer.open(file_two.c_str(), std::ios::in);

		unsigned int count = 0;
		int one_flag, two_flag, type_flag;
		one_flag = two_flag = 1;
		std::vector<std::string> file_one_row;
		std::vector<std::string> file_two_row;
		std::string filename, one_temp, two_temp;

		std::stringstream ss;
		ss << file_id;
		filename = type + "_" + ss.str();

		if (type.compare("dept") == 0)
			type_flag = 3;
		if (type.compare("emp") == 0)
			type_flag = 0;

		File *page = NULL;
		while (this->first_buffer.is_open() || this->second_buffer.is_open()) {
			if (one_flag && this->first_buffer.is_open()) {
				std::getline(this->first_buffer, one_temp);
				read(file_one_row, this->first_buffer, one_temp);
			}
			if (two_flag && this->second_buffer.is_open()) {
				std::getline(this->second_buffer, two_temp);
				read(file_two_row, this->second_buffer, two_temp);
			}

			if (page == NULL)
				page = new File(filename);

			// !!!!DANGER!!!!
			// Bad Hack....
			// This is a sin.
			// I must repent.
			// For the love of all that is right, plz fix later.
			if (file_one_row.size() == 0){
				for (int i = 0; i < 4; i++)
					file_one_row.push_back("9999999");
			}
			if (file_two_row.size() == 0){
				for (int i = 0; i < 4; i++)
					file_two_row.push_back("9999999");
			}

			if (file_two_row[type_flag].compare("9999999") == 0 && file_one_row[type_flag].compare("9999999") == 0) {
				this->first_buffer.close();
				this->second_buffer.close();
				continue;
			}
			// End of bad hack.

			if (file_compare(file_one_row[type_flag], file_two_row[type_flag])) {
				addToPage(page, file_one_row);
				file_one_row.clear();
				one_flag = 1;
				two_flag = 0;
			}
			else {
				addToPage(page, file_two_row);
				file_two_row.clear();
				one_flag = 0;
				two_flag = 1;
			}

			count++;

			if (count == block_size) {
				page->save();
				delete page;
				page = NULL;
			}
		}
		if (page != NULL) {
			page->save();
			delete page;
			page = NULL;
		}
		first_buffer.close();
		second_buffer.close();
		return filename;
	}

	bool removefile(std::string &file_name){
		char buffer[file_name.length()];
		strcpy(buffer, file_name.c_str());
		if (remove(buffer) != 0)
			return true;
		return false;
	}

	bool renamefile(std::string &file_name, std::string new_name){
		char buffer_old[file_name.length()];
		char buffer_new[new_name.length()];

		strcpy(buffer_old, file_name.c_str());
		strcpy(buffer_new, new_name.c_str());

		if (rename(buffer_old, buffer_new))
			return true;
		return false;
	}

	std::string merge_files() {
		std::string merged_file;
		std::string first_file;
		std::string second_file;

		int type_flag = 0;
		if (type.compare("dept") == 0)
			type_flag = 1;
		if (type.compare("emp") == 0)
			type_flag = 0;

		if (total <= 1){
			std::stringstream ss;
			first_file = fetch_file_name(1);
			ss << type_flag;
			if (renamefile(first_file, type + "_" + ss.str())){
				perror("Error renaming file.\n");
				exit(EXIT_FAILURE);
			}
			return type + "_" + ss.str();
		}

		// First Pass.
		first_file = fetch_file_name(1);
		second_file = fetch_file_name(2);
		merged_file = merge(first_file, second_file, 0);

		// Only for Unix like systems.
		if (removefile(first_file) || removefile(second_file))
			perror("Error deleting file.\n");

		// Second Pass
		int i;
        std::string temp;
		for (i = 3; i <= total; i++) {
			second_file = fetch_file_name(i);
            temp = merged_file;
			merged_file = merge(merged_file, second_file, i);
			if (removefile(second_file) != 0 || removefile(temp) != 0)
				perror("Error deleting file.\n");
		}
		first_buffer.close();
		second_buffer.close();
		return merged_file;
	}
};

class JoinTables{
	private:
		int block_size;
		std::string file_one_name;
		std::string file_two_name;
		std::fstream file_one;
		std::fstream file_two;
		std::fstream fout;
		std::vector<Emp*> emp;
		std::vector<Dept*> dept;
	
	void read(std::vector<std::string> &row, std::fstream &name, std::string &temp) {
		std::string line, word, filename;

		//std::getline(name, line);
		std::stringstream ss(temp);

		while (getline(ss, word, ',')) {
			row.push_back(word);
		}
	}
	void save(){
		std::ofstream fout("join.csv", std::ios::app);
		fout << "id" << ","
			<< "ename" << ","
			<< "age" << ","
			<< "salary" << ","
			<< "did" << ","
			<< "dname" << ","
			<<  "budget" << "\n";
		for (size_t i = 0; i < emp.size(); i++){
			fout << emp[i]->get_eid() << ","
				<< emp[i]->get_ename() << ","
				<< emp[i]->get_age() << ","
				<< emp[i]->get_salary() << ","
				<< dept[i]->get_did() << ","
				<< dept[i]->get_dname() << ","
				<< dept[i]->get_budget() << "\n";
				delete emp[i];
				delete dept[i];
		}
		emp.clear();
		dept.clear();
		fout.close();
	}

	public:
		JoinTables(std::string name_one, std::string name_two, int block_size){
			file_one_name = name_one;
			file_two_name = name_two;
			this->block_size = block_size;
		}
		// I Just want this to be done, so this is not going to be clean....
		void join(){
			file_two.open(file_two_name.c_str(), std::ios::in);
			
			int flag, temp;
			double a = 0;
			std::vector<std::string> file_one_row;
			std::vector<std::string> file_two_row;
			std::string one_temp, two_temp;

			Emp *e = NULL;
			Dept *d = NULL;

			flag = 1;
			
			
			while (file_two.is_open()) {
				std::getline(file_two, two_temp);
				read(file_two_row, file_two, two_temp);

				file_one.open(file_one_name.c_str(), std::ios::in);	
				while (file_one.is_open()){
					std::getline(file_one, one_temp);
					read(file_one_row, file_one, one_temp);
					
					if (atoi(file_one_row[0].c_str()) == atoi(file_two_row[3].c_str())){
						file_one.close();
						continue;
					}
					if (file_one.eof())
						file_one.close();

					file_one_row.clear();
				}

				if (atoi(file_one_row[0].c_str()) == atoi(file_two_row[3].c_str())){
					std::stringstream ss;
					ss << file_one_row[3];
					ss >> a;
					e = new Emp(atoi(file_one_row[0].c_str()), atoi(file_one_row[2].c_str()), a, file_one_row[1]);

					ss << file_two_row[2];
					ss >> a;
					d = new Dept(atoi(file_two_row[0].c_str()), atoi(file_two_row[3].c_str()), a, file_two_row[1]);

					emp.push_back(e);
					dept.push_back(d);

					e = NULL;
					d = NULL;

					file_one_row.clear();
					file_two_row.clear();
				}
				
				if(file_two.eof()){
					file_two.close();
					file_one.close();
				}

				if (emp.size() == dept.size() && emp.size() == block_size){
					save();
				}

			}
			if (emp.size() > 0)
				save();
			
			file_one.close();
			file_two.close();
		}


};

int main()
{
	int memory_size = 22;
	std::string sorted_emp;
	std::string sorted_dept;

	FileHandler dept_file("Dept.csv", "dept", memory_size);
	FileHandler emp_file("Emp.csv", "emp", memory_size);

	int total_dept = dept_file.read_table();
	int total_emp = emp_file.read_table();

	BufferHandler buffer_handler_emp("tempemp_", "emp", total_emp, memory_size);
	BufferHandler buffer_handler_dept("tempdept_", "dept", total_dept, memory_size);

	sorted_emp = buffer_handler_emp.merge_files();
	sorted_dept = buffer_handler_dept.merge_files();

	JoinTables tables(sorted_emp, sorted_dept, memory_size);
	
	tables.join();
	buffer_handler_emp.removefile(sorted_emp);
	buffer_handler_dept.removefile(sorted_dept);

	return 0;
}
