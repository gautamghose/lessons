#include <iostream>
#include <vector>

using namespace std;

struct part_answer {
  int best_value;
  vector<int> apts;
  part_answer() : best_value(-1) {}
  part_answer(int val, const vector<int>& a) : best_value(val), apts(a) {}
  part_answer(const part_answer& right) {
    this->best_value = right.best_value;
    this->apts = right.apts;
  }
};

int get_best_sequence_starting_at(const vector<int>& appointments, size_t start_idx, vector<int>& best_sequence, vector<part_answer>& memo)
{
  if (start_idx >= appointments.size()) {
    best_sequence.clear();
    return 0;
  }
  
  if (memo[start_idx].best_value != -1) {
    best_sequence = memo[start_idx].apts;
    return memo[start_idx].best_value;
  }
  
  vector<int> best_sequence_with, best_sequence_without;
  int max_with_this = get_best_sequence_starting_at(appointments, start_idx+2, best_sequence_with, memo);
  max_with_this += appointments[start_idx];
  int max_without_this = get_best_sequence_starting_at(appointments, start_idx+1, best_sequence_without, memo);
  int best_value = max_with_this;
  if (max_with_this > max_without_this) {
    best_sequence = best_sequence_with;
    best_sequence.push_back(appointments[start_idx]);
    best_value = max_with_this;
  } else {
    best_sequence = best_sequence_without;
    best_value = max_without_this;
  }
  memo[start_idx] = part_answer(best_value, best_sequence);
  return best_value;
}



int get_best_sequence(const vector<int>& appointments, vector<int>& best_sequence)
{
  std::vector<part_answer> memo(appointments.size());
  return get_best_sequence_starting_at(appointments, 0, best_sequence, memo);
}


int get_best_sequence_iter(const vector<int>& appointments, vector<int>& best_sequence)
{
  //I will start from the back. If I know the best_sequence starting at [idx+1] and [idx+2]
  //then, the best_sequence at [idx] will be the maximum of [idx]+[idx+2] or [idx+1]
  int bestseq_at_plus_2 = 0;
  int bestseq_at_plus_1 = 0;
  int best_seq_here = 0;
  vector<int> enum_best_seq_at_plus_2;
  vector<int> enum_best_seq_at_plus_1;
  vector<int> enum_best_seq_here;
  for (int i=appointments.size()-1; i>=0; --i) {
    best_seq_here = appointments[i]+bestseq_at_plus_2;
    enum_best_seq_here = enum_best_seq_at_plus_2;
    enum_best_seq_here.push_back(appointments[i]);
    if (best_seq_here < bestseq_at_plus_1) {
      best_seq_here = bestseq_at_plus_1; //i.e. it is better to not take this one at [i]
      enum_best_seq_here = enum_best_seq_at_plus_1;
    }
    //cout << "best seq at " << i << " is " << best_seq_here << endl;
    bestseq_at_plus_2 = bestseq_at_plus_1;
    enum_best_seq_at_plus_2 = enum_best_seq_at_plus_1;
    bestseq_at_plus_1 = best_seq_here;
    enum_best_seq_at_plus_1 = enum_best_seq_here;
  }
  best_sequence = enum_best_seq_here;
  return best_seq_here;
}

int main(int argc, char** argv)
{
  int n;
  cin >> n;
  vector<int> apptmts(n);
  for (int i=0; i<n; ++i) {
    cin >> apptmts[i];
  }
  vector<int> best_sequence;
  int best_val = get_best_sequence_iter(apptmts, best_sequence);
  cout << "Best val: " << best_val << endl;
  for (auto a: best_sequence) {
    cout << a << endl;
  }
  return 0;
}
