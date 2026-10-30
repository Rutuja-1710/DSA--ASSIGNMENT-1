# ---------- helpers ----------
def to_str(a, length=None):
    if length is None:
        length = len(a)
    return "[" + ", ".join(str(x) for x in a[:length]) + "]"
 
 
def pair_str(p):
    return f"({p[0]},{p[1]})"
 
 
# =====================================================================
# PART A: Remove duplicates from a sorted array
# =====================================================================
 
def remove_duplicates_reference(a):
    """Reference: builds a separate list; for each value, checks whether it was
    already added.  Time O(n*k), auxiliary space O(k)."""
    result = []
    for x in a:
        already_added = False
        for y in result:
            if y == x:
                already_added = True
                break
        if not already_added:
            result.append(x)
    return result  # len(result) is k
 
 
def remove_duplicates_optimized(a):
    """Optimized: slow marks the end of the unique prefix, fast scans the array.
    Time O(n), auxiliary space O(1).  Modifies a in place, returns k."""
    if len(a) == 0:                 # empty array: nothing to do
        return 0
    slow = 0                        # last position of the unique prefix
    for fast in range(1, len(a)):
        if a[fast] != a[slow]:      # new distinct value found
            slow += 1
            a[slow] = a[fast]       # place it right after the prefix
        # else: duplicate, just skip it
    return slow + 1                 # number of distinct values
 
 
def trace_remove_duplicates(inp):
    a = list(inp)
    print(f"Dry run (remove duplicates) on {to_str(a)}")
    if not a:
        print("  empty array -> k = 0\n")
        return
    print(f"{'Step':<6}{'slow':<6}{'fast':<6}{'a[fast]':<9}{'a[slow]':<9}{'Action':<30}Array after step")
    slow = 0
    step = 0
    for fast in range(1, len(a)):
        step += 1
        vf, vs, slow_before = a[fast], a[slow], slow
        if vf != vs:
            slow += 1
            a[slow] = a[fast]
            action = f"differ: slow={slow}, a[slow]={vf}"
        else:
            action = "same: skip"
        print(f"{step:<6}{slow_before:<6}{fast:<6}{vf:<9}{vs:<9}{action:<30}{to_str(a)}")
    print(f"  k = {slow + 1}, unique prefix = {to_str(a, slow + 1)}\n")
 
 
def run_duplicate_tests():
    tests = [
        ([1, 1, 2, 2, 2, 3, 4, 4], [1, 2, 3, 4]),
        ([5], [5]),
        ([], []),
        ([-3, -3, -1, 0, 0], [-3, -1, 0]),
        ([7, 7, 7, 7], [7]),
        ([1, 2, 3], [1, 2, 3]),
    ]
    print(f"{'No.':<5}{'Input':<26}{'k':<5}{'Prefix (optimized)':<20}{'Expected prefix':<18}{'Ref ok':<10}Result")
    for no, (inp, exp) in enumerate(tests, start=1):
        ref = remove_duplicates_reference(inp)
        work = list(inp)
        k = remove_duplicates_optimized(work)
        opt_ok = (k == len(exp)) and work[:k] == exp
        ref_ok = (ref == exp)
        print(f"{no:<5}{to_str(inp):<26}{k:<5}{to_str(work, k):<20}{to_str(exp):<18}"
              f"{'yes' if ref_ok else 'NO':<10}{'PASS' if (opt_ok and ref_ok) else 'FAIL'}")
    print()
 
 
# =====================================================================
# PART B: Pair sum in a sorted array
# =====================================================================
 
def pair_sum_brute_force(a, target):
    """Brute force: try every pair i < j.  Time O(n^2), auxiliary space O(1)."""
    n = len(a)
    for i in range(n):
        for j in range(i + 1, n):
            if a[i] + a[j] == target:
                return (i, j)
    return (-1, -1)
 
 
def pair_sum_two_pointer(a, target):
    """Optimized: pointers at both ends.  Time O(n), auxiliary space O(1)."""
    left, right = 0, len(a) - 1
    while left < right:             # never uses the same element twice
        s = a[left] + a[right]
        if s == target:
            return (left, right)
        elif s < target:
            left += 1               # need a larger sum
        else:
            right -= 1              # need a smaller sum
    return (-1, -1)
 
 
def trace_pair_sum(a, target):
    print(f"Dry run (pair sum) on {to_str(a)}, T = {target}")
    print(f"{'Step':<6}{'left':<6}{'right':<7}{'a[l]':<8}{'a[r]':<8}{'sum':<7}Action")
    l, r, step = 0, len(a) - 1, 0
    while l < r:
        step += 1
        s = a[l] + a[r]
        ll, rr = l, r
        if s == target:
            action = f"sum == T: found pair ({l},{r})"
        elif s < target:
            action = "sum < T: left++"
            l += 1
        else:
            action = "sum > T: right--"
            r -= 1
        print(f"{step:<6}{ll:<6}{rr:<7}{a[ll]:<8}{a[rr]:<8}{s:<7}{action}")
        if s == target:
            print()
            return
    print("  left >= right: stop, no pair -> (-1,-1)\n")
 
 
def valid_pair(a, t, p, exists):
    if not exists:
        return p == (-1, -1)
    return 0 <= p[0] < p[1] < len(a) and a[p[0]] + a[p[1]] == t
 
 
def run_pair_tests():
    tests = [
        ([1, 2, 4, 7, 11, 15], 15, True),
        ([1, 3, 5, 8], 6, True),     # pair (0,2): 1+5
        ([1, 3, 5, 8], 7, False),    # T=7 as printed in the handout: no pair exists
        ([2, 4, 6], 20, False),
        ([1, 2, 2, 3], 4, True),
        ([], 5, False),
        ([5], 10, False),
        ([3, 3], 6, True),
        ([-4, -1, 0, 2, 9], 8, True),
    ]
    print(f"{'No.':<5}{'Array':<22}{'T':<5}{'Brute':<11}{'Two-ptr':<11}{'Expected':<14}Result")
    for no, (a, t, exists) in enumerate(tests, start=1):
        b = pair_sum_brute_force(a, t)
        o = pair_sum_two_pointer(a, t)
        ok = valid_pair(a, t, b, exists) and valid_pair(a, t, o, exists)
        print(f"{no:<5}{to_str(a):<22}{t:<5}{pair_str(b):<11}{pair_str(o):<11}"
              f"{'valid pair' if exists else '(-1,-1)':<14}{'PASS' if ok else 'FAIL'}")
    print()
 
 
if __name__ == "__main__":
    print("===== PART A: Remove duplicates from a sorted array =====")
    run_duplicate_tests()
    trace_remove_duplicates([1, 1, 2, 2, 2, 3, 4, 4])
 
    print("===== PART B: Pair sum in a sorted array =====")
    run_pair_tests()
    trace_pair_sum([1, 2, 4, 7, 11, 15], 15)
    trace_pair_sum([2, 4, 6], 20)
 