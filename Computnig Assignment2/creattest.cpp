#include "creattest.h"
// function to generate string of certain length
string randstr(int n)
{
    std::string str = "";
    for (int i = 1; i <= n; i++) {
        int flag;
        // choose the upper case or lower case randomly
        flag = rand() % 2;
        if (flag == 1)
            str += rand() % ('Z' - 'A' + 1) + 'A';
        else
            str += rand() % ('z' - 'a' + 1) + 'a';
    }
    return str;
}
// generate int in certain range
int randnum(int min, int max)
{
    int num = rand() % (max - min) + min;
    return num;
}
// generate float in certain range
float randfloat(float max, long unsigned int seed)
{
    default_random_engine e{seed};
    uniform_real_distribution<double> u(0, max);
    return u(e);
}

int creattest(int months, int num)
{
    // creat a folder containing daily data

    string folderpath = "./patients";
    string command;
    command = "mkdir -p " + folderpath;
    system(command.c_str());

    // use loop to creat the data per day
    for (int m = 1; m <= months; m++) {
        for (int d = 1; d < 31; d++) {
            for (int t = 0; t <= 1; t++) {
                // a single file contains all the patients come in half of the day,
                // say, before the 12:00 AM or after
                int date = m * 1000 + d * 10 + t;
                string name = "patients/" + to_string(date) + ".csv";
                ofstream testcase;
                testcase.open(name);
                testcase << "Registry Identifier,"
                         << "Registration Year,Month,Day,Hour,Minute,";
                testcase << "Name"
                         << ","
                         << "Phone"
                         << ","
                         << "WeChat"
                         << ","
                         << "E-mail"
                         << ","
                         << "Identification Number"
                         << ",";
                testcase << "Address_x"
                         << ","
                         << "Address_y"
                         << ","
                         << "Profession"
                         << ","
                         << "Risk"
                         << ","
                         << "Birth Year"
                         << ",";
                testcase << "Month"
                         << ","
                         << "Day"
                         << ","
                         << "Deadline Year"
                         << ","
                         << "Month"
                         << ","
                         << "Day"
                         << ","
                         << "Treat Type" << endl;

                int num_today = rand() % num;
                for (int i = 0; i < num_today; i++) {
                    int identifier = rand() % 225;

                    int reg_year = YEAR;
                    int reg_month = m;
                    int reg_day = d;

                    int reg_hour;
                    if (t == 0) {
                        reg_hour = rand() % 13;
                    }
                    else {
                        reg_hour = rand() % 12 + 12;
                    }

                    int reg_minute = rand() % 60;

                    // less useful info
                    string name = randstr(10);
                    string phone = randstr(10);
                    string wechat = randstr(10);
                    string email = randstr(10);
                    int identification = randnum(10000000, 99999999);

                    float addr[2];
                    addr[0] = randfloat(BOUNDARY, identification);
                    addr[1] = randfloat(BOUNDARY, identification / 2);

                    int profession = rand() % 7 + 1;
                    int risk = rand() % 4;

                    int birth_year = rand() % (YEAR - 1900) + 1900;
                    int birth_month = rand() % 12 + 1;
                    int birth_day = rand() % 31 + 1;

                    int percent = 5;  // assume 5% patients have ddl
                    int ddl_flag = rand() % 101;

                    int ddl_year;
                    int ddl_month;
                    int ddl_day;

                    if (ddl_flag < percent) {
                        ddl_year = YEAR;
                        ddl_month = rand() % (13 - reg_month) + reg_month;
                        ddl_day = rand() % (31 - reg_day) + reg_day;
                    }
                    else {
                        ddl_year = 0;
                        ddl_month = 0;
                        ddl_day = 0;
                    }

                    int type = rand() % 3;
                    // output to coresponding file
                    testcase << identifier << "," << reg_year << "," << reg_month << "," << reg_day << "," << reg_hour
                             << "," << reg_minute << ",";
                    testcase << name << "," << phone << "," << wechat << "," << email << "," << identification << ","
                             << addr[0] << ",";
                    testcase << addr[1] << "," << profession << "," << risk << "," << birth_year << "," << birth_month
                             << "," << birth_day << ",";
                    testcase << ddl_year << "," << ddl_month << "," << ddl_day << "," << type << endl;
                }
                testcase.close();
            }
        }
    }
    return 1;
}
