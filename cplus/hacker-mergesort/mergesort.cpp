#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the countInversions function below.
static long nr_inversions = 0;
void merge(vector<int>& sorted_arr, size_t left_start, size_t left_end, size_t right_start, size_t right_end)
{
    //we need a scratch array of size (right_end-left_start+1)
    assert(left_end>=left_start);
    assert(right_start==left_end+1);
    assert(right_end>=right_start);
    size_t scratch_arr_size = right_end-left_start+1;
    if (scratch_arr_size <= 1) return;
    std::vector<int> scratch(scratch_arr_size);
    size_t l = left_start, r = right_start, i = 0;
    while (l <= left_end && r <= right_end) {
        while ((sorted_arr[l] <= sorted_arr[r]) && (l<=left_end)) {
            scratch[i] = sorted_arr[l];
            i++; l++;
        }
        if (l > left_end) {
            //finished the left-array. So, just copy the right array
            while (r <= right_end) {
                scratch[i] = sorted_arr[r];
                i++; r++;
            }
        } else {
            assert(sorted_arr[l]>sorted_arr[r]);
            scratch[i] = sorted_arr[r];
            i++; r++; 
            nr_inversions += (left_end-l+1); // the 'right' entry jumped so many on the left
            if (r > right_end) {
                while (l <= left_end) {
                    scratch[i] = sorted_arr[l];
                    i++; l++;
                }
            }
        }
    }
    assert(l>left_end);
    assert(r>right_end);
    assert(i == scratch_arr_size);
    size_t idx = 0;
    for (const auto& val: scratch) {
        sorted_arr[left_start+idx] = val;
        idx ++;
    }
    return;
}
void merge_sort(vector<int>& sorted_arr, size_t start, size_t end)
{
    if (start >= end) return;
    size_t halfway = (start+end)/2;
    merge_sort(sorted_arr, start, halfway);
    merge_sort(sorted_arr, halfway+1, end);
    merge(sorted_arr, start, halfway, halfway+1, end);
}
long countInversions(const vector<int>& arr) {
    vector<int> sorted(arr);
    nr_inversions = 0;
    if (arr.size() < 1) {
        return 0;
    }
    merge_sort(sorted, 0, arr.size()-1);
#if 1
    for (const auto& val: sorted) {
        cout << val << " ";
    }
#endif
    cout << std::endl;
    return nr_inversions;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string arr_temp_temp;
        getline(cin, arr_temp_temp);

        vector<string> arr_temp = split_string(arr_temp_temp);

        vector<int> arr(n);

        for (int i = 0; i < n; i++) {
            int arr_item = stoi(arr_temp[i]);

            arr[i] = arr_item;
        }

        long result = countInversions(arr);

        fout << result << "\n";
    }

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

