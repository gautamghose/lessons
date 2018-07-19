#include <bits/stdc++.h>
#include <math.h>
#include <assert.h>

using namespace std;

vector<string> split_string(string);

/*
 * Complete the movingTiles function below.
 */
double my_sqrt(double x)
{
    return (double) sqrt(x);
}

vector<double> movingTiles(int l, int s1, int s2, vector<long> queries) {
    /*
     * Write your code here.
     */
    if (s2 < s1) {
        int tmp = s1;
        s1 = s2;
        s2 = tmp;
    }
    assert(s2>s1);
    /*
    double top = l + r1*T;
    double right = l + r1*T;
    double bottom = 0 + r2*T;
    double left = 0 + r2*T;
    */
    /*
    long double speed_delta = get_projected_speed(s2-s1);
    area = (top-bottom)*(right-left) = (l-(r2-r1)*T)*(l-(r2-r1)*T) = (l - d*T)^2
    so,
    T = (l-sqrt(area))/(speed_delta);
    */
    auto it = queries.begin();
std::cerr << "max_long: " << std::numeric_limits<long>::max() << std::endl;
    vector<double> answers;
    while (it!=queries.end()) {
        long area = (*it);
std::cerr << area << std::endl;
        double q = my_sqrt((double)area);
        assert((double)l >= q);
        long double one_answer = ((long double)l - q)*(my_sqrt(2.0))/(long double)(s2-s1);
        long double max_double = (long double) std::numeric_limits<double>::max();
        if (one_answer > max_double)
            answers.push_back((double)max_double);
        else
            answers.push_back((double)one_answer);
        it ++;
    }
    return answers;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string lS1S2_temp;
    getline(cin, lS1S2_temp);

    vector<string> lS1S2 = split_string(lS1S2_temp);

    int l = stoi(lS1S2[0]);

    int s1 = stoi(lS1S2[1]);

    int s2 = stoi(lS1S2[2]);

    int queries_count;
    cin >> queries_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<long> queries(queries_count);

    for (int queries_itr = 0; queries_itr < queries_count; queries_itr++) {
        long queries_item;
        cin >> queries_item;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        queries[queries_itr] = queries_item;
    }

    vector<double> result = movingTiles(l, s1, s2, queries);

    //fout.precision(std::numeric_limits< double >::max_digits10 + 3);
    fout.precision(20);
    for (int result_itr = 0; result_itr < result.size(); result_itr++) {
        fout << std::fixed << result[result_itr];

        if (result_itr != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

