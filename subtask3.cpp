#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <omp.h>

using namespace std;
using namespace std::chrono;
using ll = long long int;

vector<pair<double, int>> best_pnl;

/*
1 -> basic_strategy
2 -> dma
3 -> improved_dma
4 -> macd
5 -> rsi
6 -> adx
7 -> linear_regression
*/
string formatingYtoD(string s)
{
    string res = "";
    string year = "";
    year += s[0];
    year += s[1];
    year += s[2];
    year += s[3];
    string month = "";
    month += s[5];
    month += s[6];
    string date = "";
    date += s[8];
    date += s[9];
    res = date + "/" + month + "/" + year;
    return res;
}
string calculateXDaysaheadhyphen(const string &startDate, int x)
{
    // Convert the start date string to time_t
    tm t = {};
    if (strptime(startDate.c_str(), "%Y-%m-%d", &t) == NULL)
    {
        cerr << "Error: Invalid start date format." << endl;
        return "";
    }
    time_t start_time = mktime(&t);

    // Create a duration of x days
    hours x_hours(24 * (x)); // 24 hours in a day

    // Subtract x days from the start time
    time_t result_time = start_time + x_hours.count() * 3600;
    if (x == 0)
    {
        result_time = start_time;
    }
    // Convert the result back to tm struct
    tm *result_tm = localtime(&result_time);

    // Convert the result tm struct to a string
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", result_tm);

    return string(buffer);
}
string calculateXDaysAgohyphen(const string &startDate, int x)
{
    // Convert the start date string to time_t
    tm t = {};
    if (strptime(startDate.c_str(), "%Y-%m-%d", &t) == NULL)
    {
        cerr << "Error: Invalid start date format." << endl;
        return "";
    }
    time_t start_time = mktime(&t);

    // Create a duration of x days
    hours x_hours(-24 * (x)); // 24 hours in a day

    // Subtract x days from the start time
    time_t result_time = start_time + x_hours.count() * 3600;
    if (x == 0)
    {
        result_time = start_time;
    }
    // Convert the result back to tm struct
    tm *result_tm = localtime(&result_time);

    // Convert the result tm struct to a string
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", result_tm);

    return string(buffer);
}
string calculateXDaysahead(const string &startDate, int x)
{
    // Convert the start date string to time_t
    tm t = {};
    if (strptime(startDate.c_str(), "%d/%m/%Y", &t) == NULL)
    {
        cerr << "Error: Invalid start date format." << endl;
        return "";
    }
    time_t start_time = mktime(&t);

    // Create a duration of x days
    hours x_hours(24 * (x)); // 24 hours in a day

    // Subtract x days from the start time
    time_t result_time = start_time + x_hours.count() * 3600;
    if (x == 0)
    {
        result_time = start_time;
    }
    // Convert the result back to tm struct
    tm *result_tm = localtime(&result_time);

    // Convert the result tm struct to a string
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", result_tm);

    return string(buffer);
}
string calculateXDaysAgo(const string &startDate, int x)
{
    // Convert the start date string to time_t
    tm t = {};
    if (strptime(startDate.c_str(), "%d/%m/%Y", &t) == NULL)
    {
        cerr << "Error: Invalid start date format." << endl;
        return "";
    }
    time_t start_time = mktime(&t);

    // Create a duration of x days
    hours x_hours(-24 * (x)); // 24 hours in a day

    // Subtract x days from the start time
    time_t result_time = start_time + x_hours.count() * 3600;
    if (x == 0)
    {
        result_time = start_time;
    }
    // Convert the result back to tm struct
    tm *result_tm = localtime(&result_time);

    // Convert the result tm struct to a string
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", result_tm);

    return string(buffer);
}
void basic_strategy(string sym, int n, int x, string sd, string ed)
{
#pragma omp critical
    {
        string pyfile = "python3 main.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " " + to_string(n) + " 1";
        system(makecall.c_str());
        string filename = sym + "1.csv";
        unordered_map<string, double> m;
        ifstream file(filename);

        double cashflow = 0;
        vector<pair<string, double>> pre_cash;

        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size())
            {
                string date = tokens[date_index];
                double close = stod(tokens[close_index]);
                m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date] != 0)
            {
                vector<pair<double, string>> v;
                string days = curr_date;
                int c = 0;
                while (c != (n + 1))
                // while (c != (n ))
                {
                    if (m[days] != 0)
                    {
                        c++;
                        v.push_back({m[days], days});
                    }
                    days = calculateXDaysAgohyphen(days, 1);
                }
                reverse(v.begin(), v.end());

                bool mono_inc = true;
                bool mono_dec = true;
                for (int i = 1; i < v.size(); i++)
                {
                    if (v[i].first <= v[i - 1].first)
                    {
                        mono_inc = false;
                    }
                    if (v[i].first >= v[i - 1].first)
                    {
                        mono_dec = false;
                    }
                }
                // check if monootonicallly increasing
                if (mono_inc && position < x)
                {
                    vector<string> rakshit;
                    position++;
                    cashflow -= m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }
                // check if monootonicallly decreasing
                if (mono_dec && position > (-1) * x)
                {
                    vector<string> rakshit;
                    position--;
                    cashflow += m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }
                pre_cash.push_back({curr_date, cashflow});
            }
            // pre_cash[curr_date]=cashflow;

            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }
        ofstream ofile;
        ofile.open("order_statistics.csv");
        // ofile.open("order_statistics_1.1.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        ofile << "Date,Order_dir,Quantity,Price" << endl;
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();
        // ofstream ofile;
        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;
        // ofile.open("daily_cashflow_1.1.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();
        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy], 1});
        ofile << cashflow + position * m[ed_copy] << endl;
        ofile.close();
    }
    return;
}
void dma(string sym, int n, int x, int p, string sd, string ed)
{
#pragma omp critical
    {
        string pyfile = "python3 main.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " " + to_string(n) + " 2";
        system(makecall.c_str());
        string filename = sym + "2.csv";
        unordered_map<string, double> m;
        ifstream file(filename);
        double cashflow = 0;
        vector<pair<string, double>> pre_cash;
        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size())
            {
                string date = tokens[date_index];
                double close = stod(tokens[close_index]);
                m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date] != 0)
            {
                vector<pair<double, string>> v;
                string days = curr_date;
                int c = 0;
                // while (c != (n + 1))
                while (c != (n))
                {
                    if (m[days] != 0)
                    {
                        c++;
                        v.push_back({m[days], days});
                    }
                    days = calculateXDaysAgohyphen(days, 1);
                }
                reverse(v.begin(), v.end());
                double sum = 0;
                double sq_sum = 0;
                // for (int i = 0; i < v.size() - 1; i++)
                for (int i = 0; i < v.size(); i++) /// yaha dekhna hai
                {
                    sum += v[i].first;
                    sq_sum += (v[i].first * v[i].first);
                }
                double avg = sum / n;
                double std_dev = sqrtf((sq_sum / n) - avg * avg);
                // cout<<curr_date<<" "<<avg<<" "<<std_dev<<endl;
                if ((m[curr_date] - avg) >= p * std_dev && position < x)
                {
                    vector<string> rakshit;
                    position++;
                    cashflow -= m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }

                if ((avg - m[curr_date]) >= p * std_dev && position > (-1) * x)
                {
                    vector<string> rakshit;
                    position--;
                    cashflow += m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }
                pre_cash.push_back({curr_date, cashflow});
            }
            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }
        ofstream ofile;
        ofile.open("order_statistics.csv");
        ofile << "Date,Order_dir,Quantity,Price" << endl;

        // ofile.open("order_statistics_1.2.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();
        // ofstream ofile;
        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;

        // ofile.open("daily_cashflow_1.2.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();
        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy], 2});
        ofile << cashflow + position * m[ed_copy] << endl;
        ofile.close();
    }
    return;
}
void improved_dma(string sym, int n, int x, int p, int max_hold_day, double c1, double c2, string sd, string ed)
{
#pragma omp critical
    {
        string pyfile = "python3 main.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " " + to_string(n) + " 3";
        system(makecall.c_str());
        string filename = sym + "3.csv";
        unordered_map<string, double> m;
        ifstream file(filename);

        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size())
            {
                string date = tokens[date_index];
                double close = stod(tokens[close_index]);
                m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        vector<pair<string, double>> pre_cash;
        vector<double> sf;
        vector<double> ama;
        vector<int> buylist;
        int top = 0;
        double cashflow = 0;
        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date] != 0)
            {
                vector<pair<double, string>> v;
                string days = curr_date;
                int c = 0;
                // while (c != (n + 1))
                while (c != (n))
                {
                    if (m[days] != 0)
                    {
                        c++;
                        v.push_back({m[days], days});
                    }
                    days = calculateXDaysAgohyphen(days, 1);
                }
                reverse(v.begin(), v.end());
                double er_num = v[v.size() - 1].first - v[0].first;
                double er_den = 0;
                for (int i = 1; i < v.size(); i++)
                {
                    er_den += abs(v[i].first - v[i - 1].first);
                }
                double er = 0;
                if (er_den != 0)
                {
                    er = er_num / er_den;
                }
                if (sf.empty())
                {
                    sf.push_back(0.5);
                }
                else
                {
                    sf.push_back(sf[sf.size() - 1] + c1 * (((((2 * er) / (c2 + 1)) - 1) / (((2 * er) / (c2 + 1)) + 1)) - sf[sf.size() - 1]));
                }

                if (ama.empty())
                {
                    ama.push_back(m[curr_date]);
                }
                else
                {
                    ama.push_back(ama[ama.size() - 1] + sf[sf.size() - 1] * (m[curr_date] - ama[ama.size() - 1]));
                }
                // cout<<curr_date<<" "<<"sf is "<<sf[sf.size()-1]<<" "<<"ama is "<<ama[ama.size()-1]<<endl;

                if ((m[curr_date] - ama[ama.size() - 1]) >= 0.01 * p && position < x)
                {
                    vector<string> rakshit;
                    position++;
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    cashflow -= m[curr_date];
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    buylist.push_back(0);
                    order_statics.push_back(rakshit);
                }
                // check if monootonicallly decreasing
                if ((ama[ama.size() - 1] - m[curr_date]) >= 0.01 * p && position > (-1) * x)
                {
                    vector<string> rakshit;
                    position--;
                    rakshit.push_back(curr_date);
                    cashflow += m[curr_date];
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }
                for (int i = 0; i < buylist.size(); i++)
                {
                    if (buylist[i] != -1)
                    {
                        buylist[i]++;
                    }
                }
                 if (top < buylist.size() && buylist[top] > max_hold_day && position > (-1) * x)
                {
                buylist[top] = -1;
                top++;
                vector<string> rakshit;
                position--;
                rakshit.push_back(curr_date);
                cashflow += m[curr_date];
                rakshit.push_back("SELL");
                rakshit.push_back("1");
                rakshit.push_back(to_string(m[curr_date]));
                if(order_statics[order_statics.size()-1][0]!=curr_date )
                {
                    order_statics.push_back(rakshit);
                }
                else if(order_statics[order_statics.size()-1][0]==curr_date && order_statics[order_statics.size()-1][1]=="SELL" )
                {
                    order_statics[order_statics.size()-1][2]="2";
                }
                  else if(order_statics[order_statics.size()-1][0]==curr_date && order_statics[order_statics.size()-1][1]=="BUY" )
                {
                    order_statics.pop_back();
                }
                
                }
                pre_cash.push_back({curr_date, cashflow});
            }
            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }
        ofstream ofile;
        // ofile.open("order_statistics_1.3.csv");
        ofile.open("order_statistics.csv");
        ofile << "Date,Order_dir,Quantity,Price" << endl;

        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();
        // ofstream ofile;
        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;

        // ofile.open("daily_cashflow_1.3.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();
        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy], 3});

        ofile << cashflow + position * m[ed_copy] << endl;
        ofile.close();
    }
    return;
}
void macd(string sym, string sd, string ed, int x)
{
#pragma omp critical
    {
        string pyfile = "python3 mainformacd.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " 4";
        system(makecall.c_str());
        string filename = sym + "4.csv";
        unordered_map<string, double> m;
        ifstream file(filename);

        double cashflow = 0;
        vector<pair<string, double>> pre_cash;
        vector<double> short_ewm;
        vector<double> long_ewm;
        // vector<double> long_ewm;
        vector<double> macd;
        vector<double> signal;

        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size())
            {
                string date = tokens[date_index];
                double close = stod(tokens[close_index]);
                m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date] != 0)
            {
                if (short_ewm.empty())
                {

                    short_ewm.push_back(m[curr_date]);
                }
                else
                {

                    short_ewm.push_back(short_ewm[short_ewm.size() - 1] + (double)(2.0 / 13.0) * (m[curr_date] - short_ewm[short_ewm.size() - 1]));
                }
                if (long_ewm.empty())
                {
                    long_ewm.push_back(m[curr_date]);
                }
                else
                {
                    long_ewm.push_back(long_ewm[long_ewm.size() - 1] + (double)(2.0 / 27.0) * (m[curr_date] - long_ewm[long_ewm.size() - 1]));
                }
                macd.push_back(-long_ewm[long_ewm.size() - 1] + short_ewm[short_ewm.size() - 1]);
                if (signal.empty())
                {
                    signal.push_back(0);
                }
                else
                {
                    signal.push_back(signal[signal.size() - 1] + (double)(1.0 / 5.0) * (macd[macd.size() - 1] - signal[signal.size() - 1]));
                }
                // cout<<short_ewm[short_ewm.size()-1]<<" "<<long_ewm[long_ewm.size()-1]<<" "<<macd[macd.size()-1]<<" "<<signal[signal.size()-1]<<endl;
                if (macd[macd.size() - 1] > signal[signal.size() - 1] && position < x)
                {
                    vector<string> rakshit;
                    position++;
                    cashflow -= m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }

                if (macd[macd.size() - 1] < signal[signal.size() - 1] && position > (-1) * x)
                {
                    vector<string> rakshit;
                    position--;
                    cashflow += m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    order_statics.push_back(rakshit);
                }
                pre_cash.push_back({curr_date, cashflow});
            }
            // pre_cash[curr_date]=cashflow;
            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }
        ofstream ofile;
        ofile.open("order_statistics.csv");
        ofile << "Date,Order_dir,Quantity,Price" << endl;

        // ofile.open("order_statistics_1.4.1.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();
        // ofstream ofile;
        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;

        // ofile.open("daily_cashflow_1.4.1.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();
        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy], 4});

        ofile << cashflow + position * m[ed_copy] << endl;
        ofile.close();
    }
    return;
}
void rsi(string sym, string sd, string ed, int n, int x, double oversold_threshold, double overbought_threshold)
{
#pragma omp critical
    {
        string pyfile = "python3 main.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " " + to_string(n) + " 5";
        system(makecall.c_str());
        string filename = sym + "5.csv";
        unordered_map<string, double> m;
        ifstream file(filename);

        double cashflow = 0;
        vector<pair<string, double>> pre_cash;

        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size())
            {
                string date = tokens[date_index];
                double close = stod(tokens[close_index]);
                m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date] != 0)
            {
                vector<pair<double, string>> v;
                string days = curr_date;
                int c = 0;
                while (c != (n + 1))
                {
                    if (m[days] != 0)
                    {
                        c++;
                        v.push_back({m[days], days});
                    }
                    days = calculateXDaysAgohyphen(days, 1);
                }
                reverse(v.begin(), v.end());

                double tot_profit = 0, tot_loss = 0;
                for (int i = 1; i < v.size(); i++)
                {
                    if (v[i].first > v[i - 1].first)
                    {
                        tot_profit += (v[i].first - v[i - 1].first);
                    }
                    if (v[i].first < v[i - 1].first)
                    {
                        tot_loss += (v[i - 1].first - v[i].first);
                    }
                }
                double avg_gain = tot_profit / n;
                double avg_loss = tot_loss / n;
                double RS = avg_gain / avg_loss;
                double RSI = 100 - 100 / (1 + RS);

                if (RSI < oversold_threshold && position < x)
                {
                    vector<string> rakshit;
                    position++;
                    cashflow -= m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    // rakshit.push_back(to_string(RSI));
                    //  rakshit.push_back(to_string(position));
                    order_statics.push_back(rakshit);
                }
                // check if monootonicallly decreasing
                if (RSI > overbought_threshold && position > -1 * (x))
                {
                    vector<string> rakshit;
                    position--;
                    cashflow += m[curr_date];
                    rakshit.push_back(curr_date);
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date]));
                    //  rakshit.push_back(to_string(RSI));
                    //  rakshit.push_back(to_string(position));
                    order_statics.push_back(rakshit);
                }
                pre_cash.push_back({curr_date, cashflow});
            }
            // pre_cash[curr_date]=cashflow;
            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }
        ofstream ofile;
        ofile.open("order_statistics.csv");
        ofile << "Date,Order_dir,Quantity,Price" << endl;

        // ofile.open("order_statistics_1.4.2.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();
        // ofstream ofile;
        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;

        // ofile.open("daily_cashflow_1.4.2.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();
        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy], 5});

        ofile << cashflow + position * m[ed_copy] << endl;
        ofile.close();
    }
    return;
}
void adx(string sym, int x, int n, double adx_threshold, string sd, string ed)
{
#pragma omp critical
    {
        string pyfile = "python3 main.py";
        string makecall = pyfile + " " + sym + " " + sd + " " + ed + " " + to_string(n) + " 6";
        system(makecall.c_str());
        string filename = sym + "6.csv";
        ifstream file(filename);
        struct storage
        {
            double close;
            double high;
            double low;
            double prevclose;
        };
        unordered_map<string, storage> m;
        double cashflow = 0;
        vector<pair<string, double>> pre_cash;

        string line;
        getline(file, line);
        stringstream header_ss(line);
        string header_cell;
        vector<string> headers;
        while (getline(header_ss, header_cell, ','))
        {
            headers.push_back(header_cell);
        }
        auto date_iter = find(headers.begin(), headers.end(), "DATE");
        auto close_iter = find(headers.begin(), headers.end(), "CLOSE");
        auto high_iter = find(headers.begin(), headers.end(), "HIGH");
        auto low_iter = find(headers.begin(), headers.end(), "LOW");
        auto prevclose_iter = find(headers.begin(), headers.end(), "PREV. CLOSE");

        int date_index = distance(headers.begin(), date_iter);
        int close_index = distance(headers.begin(), close_iter);
        int high_index = distance(headers.begin(), high_iter);
        int low_index = distance(headers.begin(), low_iter);
        int prevclose_index = distance(headers.begin(), prevclose_iter);

        while (getline(file, line))
        {
            stringstream ss(line);
            string cell;
            vector<string> tokens;
            while (getline(ss, cell, ','))
            {
                tokens.push_back(cell);
            }
            if (date_index < tokens.size() && close_index < tokens.size() && high_index < tokens.size() && low_index < tokens.size() && prevclose_index < tokens.size())
            {
                string date = tokens[date_index];
                m[date].close = stod(tokens[close_index]);
                m[date].high = stod(tokens[high_index]);
                m[date].low = stod(tokens[low_index]);
                m[date].prevclose = stod(tokens[prevclose_index]);
                // m[date] = close;
            }
        }

        file.close();
        string curr_date = sd;
        curr_date = calculateXDaysAgo(curr_date, 0);
        string ed_copy = calculateXDaysAgo(ed, 0);
        bool endc = false;
        int position = 0;
        vector<vector<string>> order_statics;
        vector<double> TR;
        vector<double> DMp;
        vector<double> DMn;
        vector<double> ATR;
        vector<double> DIp;
        vector<double> DIn;
        vector<double> DX;
        vector<double> ADX;
        string bcd = calculateXDaysAgohyphen(curr_date, 1); // working day just prev to sd
        while (m[bcd].close == 0)
        {
            bcd = calculateXDaysAgohyphen(bcd, 1);
        }
        double high0 = (m[bcd].high);
        double low0 = (m[bcd].low);

        while (!endc)
        {
            if (curr_date == ed_copy)
            {
                endc = true;
            }
            if (m[curr_date].close != 0)
            {
                double temp = max(m[curr_date].high - m[curr_date].low, m[curr_date].high - m[curr_date].prevclose);
                temp = max(temp, m[curr_date].low - m[curr_date].prevclose);
                TR.push_back(temp);
                DMp.push_back(max((double)0, m[curr_date].high - high0));
                high0 = m[curr_date].high;
                DMn.push_back(max((double)0, m[curr_date].low - low0));
                low0 = m[curr_date].low;
                if (ATR.empty())
                {
                    ATR.push_back(TR[0]);
                }
                else
                {
                    ATR.push_back(ATR[ATR.size() - 1] + (2.0) * (TR[TR.size() - 1] - ATR[ATR.size() - 1]) / ((double)(n + 1)));
                }
                if (DIp.empty())
                {
                    DIp.push_back((DMp[0]) / (ATR[0]));
                }
                else
                {
                    DIp.push_back(DIp[DIp.size() - 1] + (2.0) * (((DMp[DMp.size() - 1]) / (ATR[ATR.size() - 1])) - DIp[DIp.size() - 1]) / ((double)(n + 1)));
                }
                if (DIn.empty())
                {
                    DIn.push_back((DMn[0]) / (ATR[0]));
                }
                else
                {
                    DIn.push_back(DIn[DIn.size() - 1] + (2.0) * (((DMn[DMn.size() - 1]) / (ATR[ATR.size() - 1])) - DIn[DIn.size() - 1]) / ((double)(n + 1)));
                }
                if ((DIp[DIp.size() - 1] + DIn[DIn.size() - 1]) == 0)
                {
                    DX.push_back(0);
                }
                else
                {
                    DX.push_back((100.0) * ((DIp[DIp.size() - 1] - DIn[DIn.size() - 1]) / (DIp[DIp.size() - 1] + DIn[DIn.size() - 1])));
                }
                if (ADX.empty())
                {
                    ADX.push_back(DX[0]);
                }
                else
                {
                    ADX.push_back(ADX[ADX.size() - 1] + (2.0) * (DX[DX.size() - 1] - ADX[ADX.size() - 1]) / ((double)(n + 1)));
                }
                // check if monootonicallly increasing
                // cout << curr_date << " " << TR[TR.size() - 1] << " " << DMp[DMp.size() - 1] << " " << DMn[DMn.size() - 1] << " " << ATR[ATR.size() - 1] << " " << DIp[DIp.size() - 1] << " " << DIn[DIn.size() - 1] << " " << DX[DX.size() - 1] << " " << ADX[ADX.size() - 1] << endl;
                if (ADX[ADX.size() - 1] > adx_threshold && position < x)
                {
                    // cout << "hi" << endl;
                    vector<string> rakshit;
                    position++;
                    cashflow -= m[curr_date].close;
                    rakshit.push_back(curr_date);
                    rakshit.push_back("BUY");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date].close));

                    order_statics.push_back(rakshit);
                }
                // check if monootonicallly decreasing
                if (ADX[ADX.size() - 1] < adx_threshold && position > (-1) * x)
                {

                    vector<string> rakshit;
                    position--;
                    cashflow += m[curr_date].close;
                    rakshit.push_back(curr_date);
                    rakshit.push_back("SELL");
                    rakshit.push_back("1");
                    rakshit.push_back(to_string(m[curr_date].close));

                    order_statics.push_back(rakshit);
                }

                pre_cash.push_back({curr_date, cashflow});
            }

            curr_date = calculateXDaysaheadhyphen(curr_date, 1);
        }

        ofstream ofile;
        ofile.open("order_statistics.csv");
        ofile << "Date,Order_dir,Quantity,Price" << endl;

        // ofile.open("order_statistics_1.4.3.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (const auto &row : order_statics)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (i == 0)
                    ofile << formatingYtoD(row[i]);
                else
                    ofile << row[i];
                if (i != row.size() - 1)
                {
                    ofile << ",";
                }
            }
            ofile << std::endl;
        }

        ofile.close();

        ofile.open("daily_cashflow.csv");
        ofile << "Date,Cashflow" << endl;

        // ofile.open("daily_cashflow_1.4.3.csv");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        for (auto i : pre_cash)
        {

            ofile << formatingYtoD(i.first) << "," << i.second << endl;
        }

        ofile.close();

        ofile.open("final_pnl.txt");
        if (!ofile.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            // return;
        }
        best_pnl.push_back({cashflow + position * m[ed_copy].close, 6});

        ofile << cashflow + position * m[ed_copy].close << endl;
        ofile.close();
    }
    return;
}
void meanreverting(string sym1, string sym2, int x, int n, double threshold, string sd, string ed)
{
    string pyfile = "python3 main.py";
    string makecall = pyfile + " " + sym1 + " " + sd + " " + ed + " " + to_string(n) + " 8";
    system(makecall.c_str());
    string filename = sym1 + "8.csv";
    double cashflow = 0;
    vector<pair<string, double>> pre_cash;
    string line;

    unordered_map<string, double> m;
    ifstream file(filename);
    getline(file, line);
    stringstream header_ss(line);
    string header_cell;
    vector<string> headers;
    while (getline(header_ss, header_cell, ','))
    {
        headers.push_back(header_cell);
    }
    auto date_iter = find(headers.begin(), headers.end(), "DATE");
    auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

    int date_index = distance(headers.begin(), date_iter);
    int close_index = distance(headers.begin(), close_iter);

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> tokens;
        while (getline(ss, cell, ','))
        {
            tokens.push_back(cell);
        }
        if (date_index < tokens.size() && close_index < tokens.size())
        {
            string date = tokens[date_index];
            double close = stod(tokens[close_index]);
            m[date] = close;
        }
    }
    file.close();

    // headers.clear();

    makecall = "";
    makecall = pyfile + " " + sym2 + " " + sd + " " + ed + " " + to_string(n) + " 8";
    system(makecall.c_str());
    filename = "";
    filename = sym2 + "8.csv";
    unordered_map<string, double> m1;
    ifstream file1(filename);
    // ifstream file(filename);
    getline(file1, line);
    stringstream header_ss1(line);
    string header_cell1;
    vector<string> headers1;
    while (getline(header_ss1, header_cell1, ','))
    {
        headers1.push_back(header_cell1);
    }
    //  date_iter = find(headers.begin(), headers.end(), "DATE");
    auto date_iter1 = find(headers1.begin(), headers1.end(), "DATE");
    //  close_iter = find(headers.begin(), headers.end(), "CLOSE");
    auto close_iter1 = find(headers1.begin(), headers1.end(), "CLOSE");

    int date_index1 = distance(headers1.begin(), date_iter1);
    int close_index1 = distance(headers1.begin(), close_iter1);

    while (getline(file1, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> tokens;
        while (getline(ss, cell, ','))
        {
            tokens.push_back(cell);
        }
        if (date_index < tokens.size() && close_index < tokens.size())
        {
            string date = tokens[date_index];
            double close = stod(tokens[close_index]);
            m1[date] = close;
        }
    }
    file1.close();

    string curr_date = sd;
    curr_date = calculateXDaysAgo(curr_date, 0);
    string ed_copy = calculateXDaysAgo(ed, 0);
    bool endc = false;
    int position = 0;
    vector<vector<string>> order_statics1;
    vector<vector<string>> order_statics2;
    int c1 = 0, c2 = 0;
    while (!endc)
    {
        if (curr_date == ed_copy)
        {
            endc = true;
        }
        if (m[curr_date] != 0 && m1[curr_date] != 0)
        {
            vector<pair<double, string>> v;
            string days = curr_date;
            int c = 0;
            while (c != (n + 1))
            {
                if (m[days] != 0 && m1[days] != 0)
                {
                    c++;
                    v.push_back({m[days] - m1[days], days});
                }
                days = calculateXDaysAgohyphen(days, 1);
            }
            reverse(v.begin(), v.end());
            double sum = 0;
            double sq_sum = 0;
            // for (int i = 0; i < v.size() - 1; i++)
            for (int i = 1; i < v.size(); i++)
            {
                sum += v[i].first;
                sq_sum += (v[i].first * v[i].first);
            }
            double avg = sum / n;
            double std_dev = sqrtf((sq_sum / n) - avg * avg);
            double zscore = ((m[curr_date] - m1[curr_date]) - (avg)) / (std_dev);
            if (zscore < (-1) * threshold && position < x)
            {
                vector<string> rakshit;
                vector<string> sahu;
                position++;
                cashflow -= m[curr_date];
                cashflow += m1[curr_date];
                rakshit.push_back(curr_date);
                rakshit.push_back("BUY");
                c1++;
                rakshit.push_back("1");
                rakshit.push_back(to_string(m[curr_date]));
                sahu.push_back(curr_date);
                sahu.push_back("SELL");
                c2--;
                sahu.push_back("1");
                sahu.push_back(to_string(m1[curr_date]));
                order_statics1.push_back(rakshit);
                order_statics2.push_back(sahu);
            }

            if (zscore > threshold && position > -x)
            {
                vector<string> rakshit;
                vector<string> sahu;
                position--;
                cashflow += m[curr_date];
                cashflow -= m1[curr_date];
                rakshit.push_back(curr_date);
                rakshit.push_back("SELL");
                c1--;
                rakshit.push_back("1");
                rakshit.push_back(to_string(m[curr_date]));
                sahu.push_back(curr_date);
                sahu.push_back("BUY");
                sahu.push_back("1");
                c2++;
                sahu.push_back(to_string(m1[curr_date]));
                order_statics1.push_back(rakshit);
                order_statics2.push_back(sahu);
            }
            pre_cash.push_back({curr_date, cashflow});
        }
        curr_date = calculateXDaysaheadhyphen(curr_date, 1);
    }
    ofstream ofile;
    ofile.open("order_statistics_1.csv");
    ofile << "Date,Order_dir,Quantity,Price" << endl;

    // ofile.open("order_statistics_1_4.csv");
    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (const auto &row : order_statics1)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (i == 0)
                ofile << formatingYtoD(row[i]);
            else
                ofile << row[i];
            if (i != row.size() - 1)
            {
                ofile << ",";
            }
        }
        ofile << std::endl;
    }

    ofile.close();
    ofile.open("order_statistics_2.csv");
    ofile << "Date,Order_dir,Quantity,Price" << endl;

    // ofile.open("order_statistics_2_4.csv");
    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (const auto &row : order_statics2)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (i == 0)
                ofile << formatingYtoD(row[i]);
            else
                ofile << row[i];
            if (i != row.size() - 1)
            {
                ofile << ",";
            }
        }
        ofile << std::endl;
    }

    ofile.close();
    // ofstream ofile;
    ofile.open("daily_cashflow.csv");
    ofile << "Date,Cashflow" << endl;

    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (auto i : pre_cash)
    {

        ofile << formatingYtoD(i.first) << "," << fixed << setprecision(2) << i.second << endl;
    }

    ofile.close();
    ofile.open("final_pnl.txt");
    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    ofile << cashflow + m[ed_copy] * c1 + c2 * m1[ed_copy] << endl;
    ofile.close();
    return;
}
void stoploss(string sym1, string sym2, int x, int n, double threshold, double sl_threshold, string sd, string ed)
{
    string pyfile = "python3 main.py";
    string makecall = pyfile + " " + sym1 + " " + sd + " " + ed + " " + to_string(n) + " 9";
    system(makecall.c_str());
    string filename = sym1 + "9.csv";
    double cashflow = 0;
    vector<pair<string, double>> pre_cash;
    string line;

    unordered_map<string, double> m;
    ifstream file(filename);
    getline(file, line);
    stringstream header_ss(line);
    string header_cell;
    vector<string> headers;
    while (getline(header_ss, header_cell, ','))
    {
        headers.push_back(header_cell);
    }
    auto date_iter = find(headers.begin(), headers.end(), "DATE");
    auto close_iter = find(headers.begin(), headers.end(), "CLOSE");

    int date_index = distance(headers.begin(), date_iter);
    int close_index = distance(headers.begin(), close_iter);

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> tokens;
        while (getline(ss, cell, ','))
        {
            tokens.push_back(cell);
        }
        if (date_index < tokens.size() && close_index < tokens.size())
        {
            string date = tokens[date_index];
            double close = stod(tokens[close_index]);
            m[date] = close;
        }
    }
    file.close();

    makecall = "";
    makecall = pyfile + " " + sym2 + " " + sd + " " + ed + " " + to_string(n) + " 9";
    system(makecall.c_str());
    filename = "";
    filename = sym2 + "9.csv";
    unordered_map<string, double> m1;
    ifstream file1(filename);

    getline(file1, line);
    stringstream header_ss1(line);
    string header_cell1;
    vector<string> headers1;
    while (getline(header_ss1, header_cell1, ','))
    {
        headers1.push_back(header_cell1);
    }

    auto date_iter1 = find(headers1.begin(), headers1.end(), "DATE");

    auto close_iter1 = find(headers1.begin(), headers1.end(), "CLOSE");

    int date_index1 = distance(headers1.begin(), date_iter1);
    int close_index1 = distance(headers1.begin(), close_iter1);

    while (getline(file1, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> tokens;
        while (getline(ss, cell, ','))
        {
            tokens.push_back(cell);
        }
        if (date_index < tokens.size() && close_index < tokens.size())
        {
            string date = tokens[date_index];
            double close = stod(tokens[close_index]);
            m1[date] = close;
        }
    }
    file1.close();
    string curr_date = sd;
    curr_date = calculateXDaysAgo(curr_date, 0);
    string ed_copy = calculateXDaysAgo(ed, 0);
    bool endc = false;
    int position = 0;
    vector<vector<string>> order_statics1;
    vector<vector<string>> order_statics2;
    vector<vector<string>> buylist;
    vector<vector<string>> selllist;
    int c1 = 0, c2 = 0;
    while (!endc)
    {
        if (curr_date == ed_copy)
        {
            endc = true;
        }
        if (m[curr_date] != 0 && m1[curr_date] != 0)
        {
            vector<pair<double, string>> v;
            string days = curr_date;
            int c = 0;
            while (c != (n + 1))
            {
                if (m[days] != 0 && m1[days] != 0)
                {
                    c++;
                    v.push_back({m[days] - m1[days], days});
                }
                days = calculateXDaysAgohyphen(days, 1);
            }
            reverse(v.begin(), v.end());
            double sum = 0;
            double sq_sum = 0;
            // for (int i = 0; i < v.size() - 1; i++)
            for (int i = 1; i < v.size(); i++)
            {
                sum += v[i].first;
                sq_sum += (v[i].first * v[i].first);
            }
            double avg = sum / n;
            double std_dev = sqrtf((sq_sum / n) - avg * avg);
            double zscore = ((m[curr_date] - m1[curr_date]) - (avg)) / (std_dev);
            int z = 0, y = 0;
            if (zscore < (-1) * threshold && position < x)
            {
            // cout<<"here"<<endl;
                // vector<string> rakshit;
                // vector<string> sahu;
                position++;
                cashflow -= m[curr_date];
                cashflow += m1[curr_date];
                // rakshit.push_back(curr_date);
                // rakshit.push_back("BUY");
                // rakshit.push_back("1");
                c1++;
                z++;
                // rakshit.push_back(to_string(m[curr_date]));
                // rakshit.push_back(to_string(position));
                // sahu.push_back(curr_date);
                // sahu.push_back("SELL");
                // sahu.push_back("1");
                c2--;
                y--;
                // sahu.push_back(to_string(m1[curr_date]));
                // sahu.push_back(to_string(position));
                // order_statics1.push_back(rakshit);
                // order_statics2.push_back(sahu);
                buylist.push_back({sym1, to_string(m[curr_date]), to_string(avg), to_string(std_dev)});
                selllist.push_back({sym2, to_string(m1[curr_date]), to_string(avg), to_string(std_dev)});
            }

            if (zscore > threshold && position > (-1) * x)
            {
                // vector<string> rakshit;
                // vector<string> sahu;
                position--;
                cashflow += m[curr_date];
                cashflow -= m1[curr_date];
                // rakshit.push_back(curr_date);
                // rakshit.push_back("SELL");
                c1--;
                z--;
                // rakshit.push_back("1");
                // rakshit.push_back(to_string(m[curr_date]));
                // sahu.push_back(curr_date);
                // sahu.push_back("BUY");
                // sahu.push_back("1");
                c2++;
                y++;
                // sahu.push_back(to_string(m1[curr_date]));
                // order_statics1.push_back(rakshit);
                // order_statics2.push_back(sahu);
                buylist.push_back({sym2, to_string(m1[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
                selllist.push_back({sym1, to_string(m[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
            }


            vector<string> rakshit;
            vector<string> sahu;
            for (int i = 0; i < buylist.size(); i++)
            {
                if (buylist[i][0] != "")
                {
                    double zsc1 = ((m[curr_date] - m1[curr_date]) - stod(buylist[i][2])) / (stod(buylist[i][3]));
                    if (zsc1 > sl_threshold && position > (-1) * x)
                    {

                        position--;

                        if (buylist[i][0] == sym1)
                        {

                            c1--;
                            z--;
                            selllist.push_back({sym1, to_string(m[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
                            cashflow += m[curr_date];
                        }
                        if (buylist[i][0] == sym2)
                        {
                            y--;
                            c2--;
                            selllist.push_back({sym2, to_string(m1[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
                            cashflow += m1[curr_date];
                        }

                        buylist[i][0] = "";
                    }
                }
            }
            for (int i = 0; i < selllist.size(); i++)
            {
                if (selllist[i][0] != "")
                {
                    double zsc1 = ((m[curr_date] - m1[curr_date]) - stod(selllist[i][2])) / (stod(selllist[i][3]));
                    if (zsc1 < (-1)*sl_threshold && position<x)
                    {

                        position++;

                        if (selllist[i][0] == sym1)
                        {

                            c1++;
                            z++;
                            buylist.push_back({sym1, to_string(m[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
                            cashflow -= m[curr_date];
                        }
                        if (selllist[i][0] == sym2)
                        {
                            y++;
                            c2++;
                            buylist.push_back({sym2, to_string(m1[curr_date]), to_string(avg), to_string(std_dev)}); // sym,price,avg,stddev
                            cashflow -= m1[curr_date];
                        }

                        selllist[i][0] = "";
                    }
                }
            }
            if (z != 0)
            {
                rakshit.push_back(curr_date);
                if(z<0)
                {
                    rakshit.push_back("SELL");
                }
                else
                {
                    rakshit.push_back("BUY");
                }
                rakshit.push_back(to_string(m[curr_date]));
                rakshit.push_back(to_string(abs(z)));
                order_statics1.push_back(rakshit);
            }
            if (y != 0)
            {
                sahu.push_back(curr_date);
                if(y<0)
                {
                    sahu.push_back("SELL");
                }
                else
                {
                    sahu.push_back("BUY");
                }
                sahu.push_back(to_string(m1[curr_date]));
                sahu.push_back(to_string(abs(y)));
                order_statics2.push_back(sahu);
            }

            pre_cash.push_back({curr_date, cashflow});
        }
        curr_date = calculateXDaysaheadhyphen(curr_date, 1);
    }
    ofstream ofile;
    // ofile.open("order_statistics_1_4.1.csv");
    ofile.open("order_statistics_1.csv");
    ofile << "Date,Order_dir,Quantity,Price" << endl;

    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (const auto &row : order_statics1)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (i == 0)
                ofile << formatingYtoD(row[i]);
            else
                ofile << row[i];
            if (i != row.size() - 1)
            {
                ofile << ",";
            }
        }
        ofile << std::endl;
    }

    ofile.close();
    ofile.open("order_statistics_2.csv");
    ofile << "Date,Order_dir,Quantity,Price" << endl;

    // ofile.open("order_statistics_2_4.1.csv");
    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (const auto &row : order_statics2)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (i == 0)
                ofile << formatingYtoD(row[i]);
            else
                ofile << row[i];
            if (i != row.size() - 1)
            {
                ofile << ",";
            }
        }
        ofile << std::endl;
    }

    ofile.close();
    // ofstream ofile;
    ofile.open("daily_cashflow.csv");
    ofile << "Date,Cashflow" << endl;

    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    for (auto i : pre_cash)
    {

        ofile << formatingYtoD(i.first) << "," << i.second << endl;
    }

    ofile.close();
    ofile.open("final_pnl.txt");
    if (!ofile.is_open())
    {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    ofile << cashflow + m[ed_copy] * c1 + c2 * m1[ed_copy] << endl;
    ofile.close();
    return;
}
struct StockData
{
    double Close;
    double Open;
    double VWAP;
    double Low;
    double High;
    double NoOfTrades;
    double Opent;
};
struct Data
{
    string date;
    double Closet;
    double Close;
    double Open;
    double VWAP;
    double Low;
    double High;
    double NoOfTrades;
    double Opent;
};
std::vector<std::vector<double>> transposeMatrix(const std::vector<std::vector<double>> &matrix)
{
    // Get the number of rows and columns in the original matrix
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    // Create a new matrix with dimensions swapped for the transpose
    std::vector<std::vector<double>> transpose(cols, std::vector<double>(rows, 0.0));

    // Populate the transpose matrix
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            transpose[j][i] = matrix[i][j];
        }
    }

    return transpose;
}
std::vector<std::vector<double>> multiplyMatrices(const std::vector<std::vector<double>> &matrix1, const std::vector<std::vector<double>> &matrix2)
{
    // Check if the matrices can be multiplied
    size_t rows1 = matrix1.size();
    size_t cols1 = matrix1[0].size();
    size_t rows2 = matrix2.size();
    size_t cols2 = matrix2[0].size();

    if (cols1 != rows2)
    {
        std::cerr << "Error: Matrices cannot be multiplied. Inner dimensions do not match." << std::endl;
        return std::vector<std::vector<double>>(); // Return an empty matrix
    }

    // Create a new matrix for the result
    std::vector<std::vector<double>> result(rows1, std::vector<double>(cols2, 0.0));

    // Perform matrix multiplication
    for (size_t i = 0; i < rows1; ++i)
    {
        for (size_t j = 0; j < cols2; ++j)
        {
            for (size_t k = 0; k < cols1; ++k)
            {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}
void printMatrix(const std::vector<std::vector<double>> &matrix);
void swapRows(std::vector<std::vector<double>> &matrix, size_t row1, size_t row2)
{
    std::swap(matrix[row1], matrix[row2]);
}
void scaleRow(std::vector<std::vector<double>> &matrix, size_t row, double scalar)
{
    for (size_t col = 0; col < matrix[row].size(); ++col)
    {
        matrix[row][col] *= scalar;
    }
}
void addMultipleOfRow(std::vector<std::vector<double>> &matrix, size_t sourceRow, size_t targetRow, double multiple)
{
    for (size_t col = 0; col < matrix[sourceRow].size(); ++col)
    {
        matrix[targetRow][col] += multiple * matrix[sourceRow][col];
    }
}
std::vector<std::vector<double>> inverseMatrix(const std::vector<std::vector<double>> &matrix)
{
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    if (rows != cols)
    {
        std::cerr << "Error: Matrix must be square to find its inverse." << std::endl;
        return std::vector<std::vector<double>>();
    }

    std::vector<std::vector<double>> augmentedMatrix(rows, std::vector<double>(2 * cols, 0.0));
    for (size_t i = 0; i < rows; ++i)
    {
        augmentedMatrix[i][i + cols] = 1.0; // Set the diagonal of the augmented matrix to 1
        for (size_t j = 0; j < cols; ++j)
        {
            augmentedMatrix[i][j] = matrix[i][j];
        }
    }

    for (size_t i = 0; i < rows; ++i)
    {
        // Make the pivot element 1
        double pivot = augmentedMatrix[i][i];
        if (pivot == 0.0)
        {
            std::cerr << "Error: Matrix is singular; inverse does not exist." << std::endl;
            return std::vector<std::vector<double>>(); // Return an empty matrix
        }
        scaleRow(augmentedMatrix, i, 1.0 / pivot);

        // Eliminate other rows
        for (size_t j = 0; j < rows; ++j)
        {
            if (j != i)
            {
                addMultipleOfRow(augmentedMatrix, i, j, -augmentedMatrix[j][i]);
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    std::vector<std::vector<double>> inverse(rows, std::vector<double>(cols, 0.0));
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            inverse[i][j] = augmentedMatrix[i][j + cols];
        }
    }

    return inverse;
}

string formatingDtoY(string s)
{
    string res = "";
    string year = "";
    year += s[6];
    year += s[7];
    year += s[8];
    year += s[9];
    string month = "";
    month += s[3];
    month += s[4];
    string date = "";
    date += s[0];
    date += s[1];
    res = year + "-" + month + "-" + date;
    return res;
}
void linear_regression(string sym, int x, int p, string train_sd, string train_ed, string sd, string ed)
{
#pragma omp critical
    {
        string pycall;
        string pyfile = "main2.py";
        pycall = "python3 " + pyfile + " " + sym + " " + train_sd + " " + train_ed;
        system(pycall.c_str());
        vector<StockData> training_data;
        ifstream ifile;
        ifile.open(sym + "LR.csv");
        string temp;
        getline(ifile, temp);
        while (getline(ifile, temp))
        {
            vector<string> tokens;
            istringstream iss(temp);
            string token;
            while (std::getline(iss, token, ','))
            {
                tokens.push_back(token);
            }
            StockData sd;
            sd.Close = stod(tokens[5]);
            sd.Open = stod(tokens[1]);
            sd.VWAP = stod(tokens[6]);
            sd.Low = stod(tokens[3]);
            sd.High = stod(tokens[2]);
            sd.NoOfTrades = stod(tokens[9]);
            sd.Opent = 0;

            training_data.push_back(sd);
            if (tokens[0] < formatingDtoY(train_sd))
                break;
        }
        vector<vector<double>> Y;
        for (int i = 1; i < training_data.size(); i++)
        {
            training_data[i].Opent = training_data[i - 1].Open;
            vector<double> temp1;
            temp1.push_back(training_data[i - 1].Close);
            Y.push_back(temp1);
        }
        training_data.erase(training_data.begin());
        vector<vector<double>> X;

        for (int i = 0; i < training_data.size(); i++)
        {
            vector<double> tp;
            tp.push_back(1.0);
            tp.push_back(training_data[i].Close);
            tp.push_back(training_data[i].Open);
            tp.push_back(training_data[i].VWAP);
            tp.push_back(training_data[i].Low);
            tp.push_back(training_data[i].High);
            tp.push_back(training_data[i].NoOfTrades);
            tp.push_back(training_data[i].Opent);
            X.push_back(tp);
        }

        vector<vector<double>> XT = transposeMatrix(X);
        vector<vector<double>> XXT = multiplyMatrices(XT, X);
        vector<vector<double>> XXTI = inverseMatrix(XXT);
        vector<vector<double>> A = multiplyMatrices(XXTI, XT);
        vector<vector<double>> B = multiplyMatrices(A, Y);
        ifile.close();

        // for (const auto& row : B) {
        //     for (const auto& value : row) {
        //         std::cout << value << " ";
        //     }
        //     std::cout << "\n";
        // }

        std::string start_date_str = sd;

        // Convert string to tm structure
        std::tm start_date = {};
        std::istringstream start_date_ss(start_date_str);
        start_date_ss >> std::get_time(&start_date, "%d/%m/%Y");

        // Convert tm structure to string in "YYYY-MM-DD" format
        std::ostringstream formatted_date_ss;
        formatted_date_ss << std::put_time(&start_date, "%Y-%m-%d");

        // Output the formatted date
        std::string formatted_sd = formatted_date_ss.str();

        ifstream bin;
        bin.open(sym + "7.csv");
        pyfile = "main3.py";
        pycall = "python3 " + pyfile + " " + sym + " " + sd + " " + ed + " 7";
        system(pycall.c_str());
        getline(bin, temp);
        vector<vector<string>> all_data;
        while (true)
        {
            getline(bin, temp);
            vector<string> tokens;
            istringstream iss(temp);
            string token;
            while (std::getline(iss, token, ','))
            {
                tokens.push_back(token);
            }
            all_data.push_back(tokens);
            if (tokens[0] < formatted_sd)
                break;
        }
        // for(int i= 0;i<all_data.size();i++){
        //     for(int j=0;j<all_data[0].size();j++){
        //         cout<<all_data[i][j]<<" ";
        //     }cout<<endl;
        // }
        vector<Data> data;
        unordered_map<string, double> mp;
        for (int i = all_data.size() - 2; i >= 0; i--)
        {
            Data sd;
            sd.date = all_data[i][0];
            sd.Closet = stod(all_data[i][5]);
            sd.Close = stod(all_data[i + 1][5]);
            sd.Open = stod(all_data[i + 1][1]);
            sd.VWAP = stod(all_data[i + 1][6]);
            sd.Low = stod(all_data[i + 1][3]);
            sd.High = stod(all_data[i + 1][2]);
            sd.NoOfTrades = stod(all_data[i + 1][9]);
            sd.Opent = stod(all_data[i][1]);
            mp[all_data[i][0]] = stod(all_data[i][5]);
            data.push_back(sd);
        }

        int positions = 0;
        unordered_map<string, double> transaction;
        double last_trading_price;
        ofstream dout;

        dout.open("order_statistics.csv");
        dout << "Date,Order_dir,Quantity,Price" << endl;

        if (!dout.is_open())
        {
            std::cerr << "Error opening file: " << std::endl;
            return;
        }
        for (int i = 0; i < data.size(); i++)
        {
            vector<double> mul;
            mul.push_back(1.0);
            mul.push_back(data[i].Close);
            mul.push_back(data[i].Open);
            mul.push_back(data[i].VWAP);
            mul.push_back(data[i].Low);
            mul.push_back(data[i].High);
            mul.push_back(data[i].NoOfTrades);
            mul.push_back(data[i].Opent);

            double close_est = 0;
            for (int j = 0; j < 8; j++)
            {
                close_est += (mul[j] * B[j][0]);
            }
            // cout<<close_est<<endl;
            double actual_price = data[i].Closet;
            double pct_change = ((close_est - actual_price) / actual_price) * 100;
            // if(data[i].date=="2024-01-31") cout<<pct_change<<endl;
            // cout<<pct_change<<endl;
            if (positions < x && pct_change >= p)
            {
                // cout<<pct_change<<endl;
                if (true)
                {
                    positions++;
                    transaction[data[i].date] = -1 * data[i].Closet;
                    // cout<<"hello"<<endl;

                    dout << formatingYtoD(data[i].date) << ","
                         << "BUY,"
                         << "1," << to_string(actual_price) << endl;
                }
            }
            else if (positions > (-1 * x))
            {
                if (pct_change <= (-1 * p))
                {
                    positions--;
                    transaction[data[i].date] = data[i].Closet;

                    dout << formatingYtoD(data[i].date) << ","
                         << "SELL,"
                         << "1," << to_string(actual_price) << endl;
                }
            }
            if (i == data.size() - 1)
            {
                last_trading_price = data[i].Closet;
            }
        }
        dout.close();
        dout.open("daily_cashflow.csv");
        dout << "Date,Cashflow" << endl;
        double total_balance = 0;

        // std::tm start_date1 = {};
        // std::istringstream start_date_ss1(sd);
        // start_date_ss1 >> std::get_time(&start_date1, "%d/%m/%Y");

        // std::tm end_date1 = {};
        // std::istringstream end_date_ss(ed);
        // end_date_ss >> std::get_time(&end_date1, "%d/%m/%Y");

        // while (std::mktime(&start_date1) <= std::mktime(&end_date1)) {

        //     std::ostringstream formatted_date_ss;
        //     formatted_date_ss << std::put_time(&start_date1, "%Y-%m-%d");
        //     std::string formatted_date = formatted_date_ss.str();

        //     total_balance += transaction[formatted_date];
        //     dout << std::put_time(&start_date1, "%d/%m/%Y") <<","<<total_balance<< "\n";
        //     start_date1.tm_mday += 1;
        //     std::mktime(&start_date1);  // Normalize the date

        // }
        for (int i = all_data.size() - 2; i >= 0; i--)
        {
            string formatted_date = all_data[i][0];
            total_balance += transaction[formatted_date];
            dout << formatingYtoD(formatted_date) << "," << to_string(total_balance) << endl;
        }

        // cout<<total_balance;
        dout.close();
        dout.open("final_pnl.txt");
        total_balance += positions * last_trading_price;
        best_pnl.push_back({total_balance, 7});

        dout << to_string(total_balance);
        dout.close();
        // cout<<"Done7";
    }
}
string oneYearAgo(string &date)
{
    std::tm input_date = {};
    std::istringstream date_ss(date);
    date_ss >> std::get_time(&input_date, "%d/%m/%Y");

    input_date.tm_year -= 1;

    std::mktime(&input_date);

    std::ostringstream result_ss;
    result_ss << std::put_time(&input_date, "%d/%m/%Y");

    return result_ss.str();
}
void best_of_all(string sym, string sd, string ed)
{
    string train_sd = oneYearAgo(sd);
    string train_ed = oneYearAgo(ed);

    // cout<<"hi\n"<<sd<<" "<<ed<<endl;;
#pragma omp parallel sections
    {
#pragma omp section
        linear_regression(sym, 5, 2, train_sd, train_ed, sd, ed);

#pragma omp section
        dma(sym, 50, 5, 2, sd, ed);

#pragma omp section
        improved_dma(sym, 14, 5, 5, 28, 2, 0.2, sd, ed);

#pragma omp section
        macd(sym, sd, ed, 5);

#pragma omp section
        rsi(sym, sd, ed, 14, 5, 30.0, 70.0);

#pragma omp section
        adx(sym, 5, 14, 25.0, sd, ed);

#pragma omp section
        basic_strategy(sym, 7, 5, sd, ed);
    }
    sort(best_pnl.begin(), best_pnl.end());
    int best_strategy = best_pnl[best_pnl.size() - 1].second;
    // cout<<best_pnl[best_pnl.size()-1].first<<" "<<best_strategy<<endl;

    if (best_strategy == 1)
    {
        basic_strategy(sym, 7, 5, sd, ed);
    }
    else if (best_strategy == 2)
    {
        dma(sym, 50, 5, 2, sd, ed);
    }
    else if (best_strategy == 3)
    {
        improved_dma(sym, 14, 5, 5, 28, 2, 0.2, sd, ed);
    }
    else if (best_strategy == 4)
    {
        macd(sym, sd, ed, 5);
    }
    else if (best_strategy == 5)
    {
        rsi(sym, sd, ed, 14, 5, 30.0, 70.0);
    }
    else if (best_strategy == 6)
    {
        adx(sym, 5, 14, 25.0, sd, ed);
    }
    else
    {
        linear_regression(sym, 5, 2, train_sd, train_ed, sd, ed);
    }
}

int main(int argc, char *argv[])
{
    // rsi("SBIN","21/02/2022", "24/08/2023",14,100,30,60);
    //  stoploss("SBIN", "ADANIENT", 5, 20, 2,4, "10/01/2022", "02/02/2023");
    // macd("SBIN","21/02/2022", "21/04/2023",100);
    // linear_regression("SBIN", 5, 2, "02/01/2023", "01/06/2023","12/12/2023", "29/01/2024");
    // best_of_all("SBIN","02/04/2023","29/09/2023");
    // basic_strategy("SBIN", 5, 2,"01/01/2023","01/01/2024");
    // macd("SBIN","02/04/2023","29/05/2023",5);
    // adx("SBIN",5,14,25.0,"02/04/2023","29/05/2023");
    // meanreverting("SBIN","ADANIENT",5,20,2,"01/01/2023","01/01/2024");
    // string S = "SBIN";
    // int x = 30;
    // double p = 5;
    // string train_start = "02/01/2023";
    // string train_end="01/06/2023";
    // string start_date = "12/12/2023";
    // string end_date = "29/12/2023";
    // string type ;
    // cout<<argc<<endl;
    // cout<<argv[2]<<endl;
    // string strategy(argv[1])
    string strategy = argv[1];
    string sym = argv[2];
    // cout<<argv[1]<<endl;
    // cout<<argv[2]<<endl;
    // cout<<argv[3]<<endl;
    // cout<<argv[4]<<endl;
    // cout<<argv[5]<<endl;
    // cout<<argv[6]<<endl;

    if (strategy.compare("BASIC") == 0)
    {
        // make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
        basic_strategy(sym, stoi(argv[4]), stoi(argv[3]), argv[5], argv[6]);
    }
    else if (strategy.compare("DMA") == 0)
    {
        // make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
        dma(sym, stoi(argv[4]), stoi(argv[3]), stoi(argv[5]), argv[6], argv[7]);
    }
    else if (strategy.compare("DMA++") == 0)
    {
        // make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
        improved_dma(sym, stoi(argv[4]), stoi(argv[3]), stoi(argv[5]), stoi(argv[6]), stod(argv[7]), stod(argv[8]), argv[9], argv[10]);
    }
    else if (strategy.compare("MACD") == 0)
    {
        // make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
        macd(sym, argv[4], argv[5], stoi(argv[3]));
    }
    else if (strategy.compare("RSI") == 0)
    {
        // make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
        rsi(sym, argv[7], argv[8], stoi(argv[4]), stoi(argv[3]), stod(argv[5]), stod(argv[6]));
    }
    else if (strategy.compare("ADX") == 0)
    {
        // make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
        adx(sym, stoi(argv[3]), stoi(argv[4]), stod(argv[5]), argv[6], argv[7]);
    }
    else if (strategy.compare("LINEAR_REGRESSION") == 0)
    {
        // make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
        linear_regression(sym, stoi(argv[3]), stoi(argv[4]), argv[5], argv[6], argv[7], argv[8]);
    }
    else if (strategy.compare("BEST_OF_ALL") == 0)
    {
        // make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
        best_of_all(sym, argv[3], argv[4]);
    }
    else if (strategy.compare("PAIRS") == 0 && argc == 9)
    {
        // make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
        meanreverting(argv[2], argv[3], stoi(argv[4]), stoi(argv[5]), stod(argv[6]), argv[7], argv[8]);
    }
    else if (strategy.compare("PAIRS") == 0 && argc == 10)
    {
        // make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
        stoploss(argv[2], argv[3], stoi(argv[4]), stoi(argv[5]), stod(argv[6]), stod(argv[7]), argv[8], argv[9]);
    }
    // else cout<<"Not able to execute any strategy";

    return 0;
}
/*
     Thank You
     Rakshit Aryan 2022CS11110 Aditya Sahu 2022CS11113
*/