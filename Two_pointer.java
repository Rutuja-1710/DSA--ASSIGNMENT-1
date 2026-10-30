
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
 
public class Two_pointer {
 
    // ---------- helpers ----------
    static String toStr(int[] a, int len) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < len; i++) {
            if (i > 0) sb.append(", ");
            sb.append(a[i]);
        }
        return sb.append("]").toString();
    }
 
    static String toStr(int[] a) { return toStr(a, a.length); }
 
    static String pairStr(int[] p) { return "(" + p[0] + "," + p[1] + ")"; }
 
    // =====================================================================
    // PART A: Remove duplicates from a sorted array
    // =====================================================================
 
    // Reference: builds a separate list; for each value, scans the list to see
    // whether it was already added.  Time O(n*k), auxiliary space O(k).
    static List<Integer> removeDuplicatesReference(int[] a) {
        List<Integer> result = new ArrayList<>();
        for (int x : a) {
            boolean alreadyAdded = false;
            for (int y : result) {
                if (y == x) { alreadyAdded = true; break; }
            }
            if (!alreadyAdded) result.add(x);
        }
        return result;  // result.size() is k
    }
 
    // Optimized: slow marks the end of the unique prefix, fast scans the array.
    // Time O(n), auxiliary space O(1).  Modifies the array in place, returns k.
    static int removeDuplicatesOptimized(int[] a) {
        if (a.length == 0) return 0;          // empty array: nothing to do
        int slow = 0;                         // last position of the unique prefix
        for (int fast = 1; fast < a.length; fast++) {
            if (a[fast] != a[slow]) {         // new distinct value found
                slow++;
                a[slow] = a[fast];            // place it right after the prefix
            }                                 // else: duplicate, just skip it
        }
        return slow + 1;                      // number of distinct values
    }
 
    static void traceRemoveDuplicates(int[] input) {
        int[] a = Arrays.copyOf(input, input.length);
        System.out.println("Dry run (remove duplicates) on " + toStr(a));
        if (a.length == 0) { System.out.println("  empty array -> k = 0\n"); return; }
        System.out.printf("%-6s%-6s%-6s%-9s%-9s%-30s%s%n",
                "Step", "slow", "fast", "a[fast]", "a[slow]", "Action", "Array after step");
        int slow = 0, step = 0;
        for (int fast = 1; fast < a.length; fast++) {
            step++;
            int vf = a[fast], vs = a[slow], slowBefore = slow;
            String action;
            if (vf != vs) {
                slow++;
                a[slow] = a[fast];
                action = "differ: slow=" + slow + ", a[slow]=" + vf;
            } else {
                action = "same: skip";
            }
            System.out.printf("%-6d%-6d%-6d%-9d%-9d%-30s%s%n",
                    step, slowBefore, fast, vf, vs, action, toStr(a));
        }
        System.out.println("  k = " + (slow + 1) + ", unique prefix = " + toStr(a, slow + 1) + "\n");
    }
 
    static void runDuplicateTests() {
        int[][] inputs = {
            {1, 1, 2, 2, 2, 3, 4, 4}, {5}, {}, {-3, -3, -1, 0, 0}, {7, 7, 7, 7}, {1, 2, 3}
        };
        int[][] expected = {
            {1, 2, 3, 4}, {5}, {}, {-3, -1, 0}, {7}, {1, 2, 3}
        };
        System.out.printf("%-5s%-26s%-5s%-20s%-18s%-10s%s%n",
                "No.", "Input", "k", "Prefix (optimized)", "Expected prefix", "Ref ok", "Result");
        for (int t = 0; t < inputs.length; t++) {
            List<Integer> ref = removeDuplicatesReference(inputs[t]);
            int[] work = Arrays.copyOf(inputs[t], inputs[t].length);
            int k = removeDuplicatesOptimized(work);
            boolean optOk = (k == expected[t].length);
            for (int i = 0; optOk && i < k; i++) optOk = (work[i] == expected[t][i]);
            boolean refOk = (ref.size() == expected[t].length);
            for (int i = 0; refOk && i < ref.size(); i++) refOk = (ref.get(i) == expected[t][i]);
            System.out.printf("%-5d%-26s%-5d%-20s%-18s%-10s%s%n",
                    t + 1, toStr(inputs[t]), k, toStr(work, k), toStr(expected[t]),
                    refOk ? "yes" : "NO", (optOk && refOk) ? "PASS" : "FAIL");
        }
        System.out.println();
    }
 
    // =====================================================================
    // PART B: Pair sum in a sorted array
    // =====================================================================
 
    // Brute force: try every pair i < j.  Time O(n^2), auxiliary space O(1).
    static int[] pairSumBruteForce(int[] a, int target) {
        int n = a.length;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (a[i] + a[j] == target) return new int[]{i, j};
        return new int[]{-1, -1};
    }
 
    // Optimized: pointers at both ends.  Time O(n), auxiliary space O(1).
    static int[] pairSumTwoPointer(int[] a, int target) {
        int left = 0, right = a.length - 1;
        while (left < right) {                    // never uses the same element twice
            long sum = (long) a[left] + a[right];
            if (sum == target) return new int[]{left, right};
            else if (sum < target) left++;        // need a larger sum
            else right--;                         // need a smaller sum
        }
        return new int[]{-1, -1};
    }
 
    static void tracePairSum(int[] a, int target) {
        System.out.println("Dry run (pair sum) on " + toStr(a) + ", T = " + target);
        System.out.printf("%-6s%-6s%-7s%-8s%-8s%-7s%s%n",
                "Step", "left", "right", "a[l]", "a[r]", "sum", "Action");
        int l = 0, r = a.length - 1, step = 0;
        while (l < r) {
            step++;
            int sum = a[l] + a[r];
            int ll = l, rr = r;
            String action;
            if (sum == target) action = "sum == T: found pair (" + l + "," + r + ")";
            else if (sum < target) { action = "sum < T: left++"; l++; }
            else { action = "sum > T: right--"; r--; }
            System.out.printf("%-6d%-6d%-7d%-8d%-8d%-7d%s%n", step, ll, rr, a[ll], a[rr], sum, action);
            if (sum == target) { System.out.println(); return; }
        }
        System.out.println("  left >= right: stop, no pair -> (-1,-1)\n");
    }
 
    static boolean validPair(int[] a, int t, int[] p, boolean exists) {
        if (!exists) return p[0] == -1 && p[1] == -1;
        return p[0] >= 0 && p[0] < p[1] && p[1] < a.length && a[p[0]] + a[p[1]] == t;
    }
 
    static void runPairTests() {
        int[][] arrays = {
            {1, 2, 4, 7, 11, 15}, {1, 3, 5, 8}, {1, 3, 5, 8}, {2, 4, 6},
            {1, 2, 2, 3}, {}, {5}, {3, 3}, {-4, -1, 0, 2, 9}
        };
        int[] targets = {15, 6, 7, 20, 4, 5, 10, 6, 8};
        boolean[] exists = {true, true, false, false, true, false, false, true, true};
        System.out.printf("%-5s%-22s%-5s%-11s%-11s%-14s%s%n",
                "No.", "Array", "T", "Brute", "Two-ptr", "Expected", "Result");
        for (int t = 0; t < arrays.length; t++) {
            int[] b = pairSumBruteForce(arrays[t], targets[t]);
            int[] o = pairSumTwoPointer(arrays[t], targets[t]);
            boolean ok = validPair(arrays[t], targets[t], b, exists[t])
                      && validPair(arrays[t], targets[t], o, exists[t]);
            System.out.printf("%-5d%-22s%-5d%-11s%-11s%-14s%s%n",
                    t + 1, toStr(arrays[t]), targets[t], pairStr(b), pairStr(o),
                    exists[t] ? "valid pair" : "(-1,-1)", ok ? "PASS" : "FAIL");
        }
        System.out.println();
    }
 
    public static void main(String[] args) {
        System.out.println("===== PART A: Remove duplicates from a sorted array =====");
        runDuplicateTests();
        traceRemoveDuplicates(new int[]{1, 1, 2, 2, 2, 3, 4, 4});
 
        System.out.println("===== PART B: Pair sum in a sorted array =====");
        runPairTests();
        tracePairSum(new int[]{1, 2, 4, 7, 11, 15}, 15);
        tracePairSum(new int[]{2, 4, 6}, 20);
    }
}
 