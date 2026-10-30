
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
using namespace std;
 
// ---------- helpers ----------
string toStr(const vector<int>& v, size_t len) {
    string s = "[";
    for (size_t i = 0; i < len; i++) {
        if (i) s += ", ";
        s += to_string(v[i]);
    }
    return s + "]";
}
string toStr(const vector<int>& v) { return toStr(v, v.size()); }
string pairStr(pair<int, int> p) {
    return "(" + to_string(p.first) + "," + to_string(p.second) + ")";
}
 
// =====================================================================
// PART A: Remove duplicates from a sorted array
// =====================================================================
 
// Reference: builds a separate list; for each value, scans the list to see
// whether it was already added.  Time O(n*k), auxiliary space O(k).
vector<int> removeDuplicatesReference(const vector<int>& a) {
    vector<int> result;
    for (int x : a) {
        bool alreadyAdded = false;
        for (int y : result) {
            if (y == x) { alreadyAdded = true; break; }
        }
        if (!alreadyAdded) result.push_back(x);
    }
    return result;  // result.size() is k
}
 
// Optimized: slow marks the end of the unique prefix, fast scans the array.
// Time O(n), auxiliary space O(1).  Modifies the array in place, returns k.
int removeDuplicatesOptimized(vector<int>& a) {
    if (a.empty()) return 0;               // empty array: nothing to do
    int slow = 0;                          // last position of the unique prefix
    for (int fast = 1; fast < (int)a.size(); fast++) {
        if (a[fast] != a[slow]) {          // new distinct value found
            slow++;
            a[slow] = a[fast];             // place it right after the prefix
        }                                  // else: duplicate, just skip it
    }
    return slow + 1;                       // number of distinct values
}
 
// Prints a dry-run table for the optimized algorithm.
void traceRemoveDuplicates(vector<int> a) {
    cout << "Dry run (remove duplicates) on " << toStr(a) << "\n";
    if (a.empty()) { cout << "  empty array -> k = 0\n"; return; }
    cout << left << setw(6) << "Step" << setw(6) << "slow" << setw(6) << "fast"
         << setw(9) << "a[fast]" << setw(9) << "a[slow]" << setw(30) << "Action"
         << "Array after step\n";
    int slow = 0, step = 0;
    for (int fast = 1; fast < (int)a.size(); fast++) {
        step++;
        int vf = a[fast], vs = a[slow];
        int slowBefore = slow;
        string action;
        if (vf != vs) {
            slow++;
            a[slow] = a[fast];
            action = "differ: slow=" + to_string(slow) + ", a[slow]=" + to_string(vf);
        } else {
            action = "same: skip";
        }
        cout << left << setw(6) << step << setw(6) << slowBefore << setw(6) << fast
             << setw(9) << vf << setw(9) << vs << setw(30) << action << toStr(a) << "\n";
    }
    cout << "  k = " << slow + 1 << ", unique prefix = " << toStr(a, slow + 1) << "\n\n";
}
 
struct DupTest { vector<int> input; vector<int> expectedPrefix; };
 
void runDuplicateTests() {
    vector<DupTest> tests = {
        {{1, 1, 2, 2, 2, 3, 4, 4}, {1, 2, 3, 4}},
        {{5}, {5}},
        {{}, {}},
        {{-3, -3, -1, 0, 0}, {-3, -1, 0}},
        {{7, 7, 7, 7}, {7}},
        {{1, 2, 3}, {1, 2, 3}},
    };
    cout << left << setw(5) << "No." << setw(26) << "Input" << setw(5) << "k" << setw(20)
         << "Prefix (optimized)" << setw(18) << "Expected prefix" << setw(10) << "Ref ok" << "Result\n";
    int no = 0;
    for (auto& t : tests) {
        no++;
        vector<int> ref = removeDuplicatesReference(t.input);
        vector<int> work = t.input;
        int k = removeDuplicatesOptimized(work);
        bool optOk = (k == (int)t.expectedPrefix.size());
        for (int i = 0; optOk && i < k; i++) optOk = (work[i] == t.expectedPrefix[i]);
        bool refOk = (ref == t.expectedPrefix);
        cout << left << setw(5) << no << setw(26) << toStr(t.input) << setw(5) << k << setw(20)
             << toStr(work, k) << setw(18) << toStr(t.expectedPrefix) << setw(10)
             << (refOk ? "yes" : "NO") << ((optOk && refOk) ? "PASS" : "FAIL") << "\n";
    }
    cout << "\n";
}
 
// =====================================================================
// PART B: Pair sum in a sorted array
// =====================================================================
 
// Brute force: try every pair i < j.  Time O(n^2), auxiliary space O(1).
pair<int, int> pairSumBruteForce(const vector<int>& a, int target) {
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] + a[j] == target) return {i, j};
    return {-1, -1};
}
 
// Optimized: pointers at both ends.  Time O(n), auxiliary space O(1).
pair<int, int> pairSumTwoPointer(const vector<int>& a, int target) {
    int left = 0, right = (int)a.size() - 1;
    while (left < right) {                       // never uses the same element twice
        long long sum = (long long)a[left] + a[right];
        if (sum == target) return {left, right};
        else if (sum < target) left++;           // need a larger sum
        else right--;                            // need a smaller sum
    }
    return {-1, -1};
}
 
void tracePairSum(const vector<int>& a, int target) {
    cout << "Dry run (pair sum) on " << toStr(a) << ", T = " << target << "\n";
    cout << left << setw(6) << "Step" << setw(6) << "left" << setw(7) << "right"
         << setw(8) << "a[l]" << setw(8) << "a[r]" << setw(7) << "sum" << "Action\n";
    int l = 0, r = (int)a.size() - 1, step = 0;
    while (l < r) {
        step++;
        int sum = a[l] + a[r];
        string action;
        int ll = l, rr = r;
        if (sum == target) action = "sum == T: found pair (" + to_string(l) + "," + to_string(r) + ")";
        else if (sum < target) { action = "sum < T: left++"; l++; }
        else { action = "sum > T: right--"; r--; }
        cout << left << setw(6) << step << setw(6) << ll << setw(7) << rr << setw(8) << a[ll]
             << setw(8) << a[rr] << setw(7) << sum << action << "\n";
        if (sum == target) { cout << "\n"; return; }
    }
    cout << "  left >= right: stop, no pair -> (-1,-1)\n\n";
}
 
struct PairTest { vector<int> a; int target; bool exists; };
 
bool validPair(const vector<int>& a, int t, pair<int, int> p, bool exists) {
    if (!exists) return p.first == -1 && p.second == -1;
    int n = (int)a.size();
    return p.first >= 0 && p.first < p.second && p.second < n && a[p.first] + a[p.second] == t;
}
 
void runPairTests() {
    vector<PairTest> tests = {
        {{1, 2, 4, 7, 11, 15}, 15, true},
        {{1, 3, 5, 8}, 6, true},     // pair (0,2): 1+5
        {{1, 3, 5, 8}, 7, false},    // T=7 as printed in the handout: no pair exists
        {{2, 4, 6}, 20, false},
        {{1, 2, 2, 3}, 4, true},
        {{}, 5, false},
        {{5}, 10, false},
        {{3, 3}, 6, true},
        {{-4, -1, 0, 2, 9}, 8, true},  // -1 + 9
    };
    cout << left << setw(5) << "No." << setw(22) << "Array" << setw(5) << "T" << setw(11)
         << "Brute" << setw(11) << "Two-ptr" << setw(14) << "Expected" << "Result\n";
    int no = 0;
    for (auto& t : tests) {
        no++;
        auto b = pairSumBruteForce(t.a, t.target);
        auto o = pairSumTwoPointer(t.a, t.target);
        bool ok = validPair(t.a, t.target, b, t.exists) && validPair(t.a, t.target, o, t.exists);
        cout << left << setw(5) << no << setw(22) << toStr(t.a) << setw(5) << t.target << setw(11)
             << pairStr(b) << setw(11) << pairStr(o) << setw(14)
             << (t.exists ? "valid pair" : "(-1,-1)") << (ok ? "PASS" : "FAIL") << "\n";
    }
    cout << "\n";
}
 
int main() {
    cout << "===== PART A: Remove duplicates from a sorted array =====\n";
    runDuplicateTests();
    traceRemoveDuplicates({1, 1, 2, 2, 2, 3, 4, 4});
 
    cout << "===== PART B: Pair sum in a sorted array =====\n";
    runPairTests();
    tracePairSum({1, 2, 4, 7, 11, 15}, 15);
    tracePairSum({2, 4, 6}, 20);
    return 0;
}
 