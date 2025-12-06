// Math Set Tests
//
//#include "Math_Set.h"
//
//using namespace std;
//
// ANSI color codes for pretty output
//#define RESET   "\033[0m"
//#define RED     "\033[31m"
//#define GREEN   "\033[32m"
//#define YELLOW  "\033[33m"
//#define BLUE    "\033[34m"
//#define MAGENTA "\033[35m"
//#define CYAN    "\033[36m"
//#define BOLD    "\033[1m"
//
// Test result tracking
//int total_tests = 0;
//int passed_tests = 0;
//int failed_tests = 0;
//
// Test helper macros
//#define TEST_START(name) \
//    std::cout << CYAN << "\n=== Testing: " << name << " ===" << RESET << std::endl; \
//    total_tests++;
//
//#define TEST_ASSERT(condition, message) \
//    if (condition) { \
//        std::cout << GREEN << "✓ PASS: " << message << RESET << std::endl; \
//        passed_tests++; \
//    } else { \
//        std::cout << RED << "✗ FAIL: " << message << RESET << std::endl; \
//        failed_tests++; \
//    }
//
//#define TEST_SECTION(name) \
//    std::cout << YELLOW << "\n--- " << name << " ---" << RESET << std::endl;
//
// Print test summary
//void print_summary()
//{
//    std::cout << BOLD << "\n" << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << BOLD << "TEST SUMMARY" << RESET << std::endl;
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << "Total Tests: " << total_tests << std::endl;
//    std::cout << GREEN << "Passed: " << passed_tests << RESET << std::endl;
//    std::cout << RED << "Failed: " << failed_tests << RESET << std::endl;
//
//    if ( failed_tests == 0 )
//    {
//        std::cout << GREEN << BOLD << "\n🎉 ALL TESTS PASSED! 🎉" << RESET << std::endl;
//    }
//    else
//    {
//        std::cout << RED << BOLD << "\n⚠️  SOME TESTS FAILED ⚠️" << RESET << std::endl;
//    }
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << "\n" << std::endl;
//}
//
// ============================================================================
// TEST 1: Basic Operations with Integers
// ============================================================================
//void test_basic_integer_operations()
//{
//    TEST_START( "Basic Integer Operations" );
//
//    Math_Set<int> set1;
//
//    TEST_SECTION( "Insertion" );
//    TEST_ASSERT( set1.Insert_Element( 5 ), "Insert 5" );
//    TEST_ASSERT( set1.Insert_Element( 2 ), "Insert 2" );
//    TEST_ASSERT( set1.Insert_Element( 8 ), "Insert 8" );
//    TEST_ASSERT( set1.Insert_Element( 1 ), "Insert 1" );
//    TEST_ASSERT( !set1.Insert_Element( 5 ), "Duplicate 5 rejected" );
//
//    std::cout << "Set after insertions: " << set1 << std::endl;
//
//    TEST_SECTION( "Properties" );
//    TEST_ASSERT( set1.Set_Cardinality() == 4, "Cardinality is 4" );
//    TEST_ASSERT( !set1.Is_Empty(), "Set is not empty" );
//    TEST_ASSERT( set1 [ 0 ] == 1, "First element is 1 (sorted)" );
//    TEST_ASSERT( set1 [ 3 ] == 8, "Last element is 8" );
//
//    TEST_SECTION( "Removal" );
//    TEST_ASSERT( set1.Remove_Element( 2 ), "Remove 2" );
//    TEST_ASSERT( set1.Set_Cardinality() == 3, "Cardinality is 3" );
//    TEST_ASSERT( !set1.Remove_Element( 99 ), "Remove non-existent element fails" );
//
//    std::cout << "Set after removal: " << set1 << std::endl;
//
//    TEST_SECTION( "Clear" );
//    set1.Clear_Set();
//    TEST_ASSERT( set1.Is_Empty(), "Set is empty after clear" );
//    TEST_ASSERT( set1.Set_Cardinality() == 0, "Cardinality is 0" );
//}
//
// ============================================================================
// TEST 2: String Operations
// ============================================================================
//void test_string_operations()
//{
//    TEST_START( "String Operations" );
//
//    Math_Set<std::string> fruits;
//
//    TEST_SECTION( "String Insertion" );
//    fruits.Insert_Element( "apple" );
//    fruits.Insert_Element( "banana" );
//    fruits.Insert_Element( "cherry" );
//    fruits.Insert_Element( "date" );
//    fruits.Insert_Element( "elderberry" );
//
//    std::cout << "Fruit set: " << fruits << std::endl;
//
//    TEST_ASSERT( fruits.Set_Cardinality() == 5, "5 fruits inserted" );
//    TEST_ASSERT( fruits [ 0 ] == "apple", "Alphabetically first: apple" );
//    TEST_ASSERT( fruits [ 4 ] == "elderberry", "Alphabetically last: elderberry" );
//
//    TEST_SECTION( "String Removal" );
//    fruits.Remove_Element( "cherry" );
//    std::cout << "After removing cherry: " << fruits << std::endl;
//    TEST_ASSERT( fruits.Set_Cardinality() == 4, "4 fruits remain" );
//
//    TEST_SECTION( "String Duplicates" );
//    TEST_ASSERT( !fruits.Insert_Element( "apple" ), "Duplicate apple rejected" );
//}
//
// ============================================================================
// TEST 3: Set Union
// ============================================================================
//void test_union_operations()
//{
//    TEST_START( "Union Operations" );
//
//    TEST_SECTION( "Integer Union" );
//    Math_Set<int> A, B;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//    A.Insert_Element( 3 );
//
//    B.Insert_Element( 3 );
//    B.Insert_Element( 4 );
//    B.Insert_Element( 5 );
//
//    Math_Set<int> C = UNION( A, B );
//
//    std::cout << "A = " << A << std::endl;
//    std::cout << "B = " << B << std::endl;
//    std::cout << "A ∪ B = " << C << std::endl;
//
//    TEST_ASSERT( C.Set_Cardinality() == 5, "Union has 5 elements" );
//    TEST_ASSERT( C [ 0 ] == 1 && C [ 1 ] == 2 && C [ 2 ] == 3 && C [ 3 ] == 4 && C [ 4 ] == 5,
//                 "Union contains {1,2,3,4,5}" );
//
//    TEST_SECTION( "String Union" );
//    Math_Set<std::string> colors1, colors2;
//    colors1.Insert_Element( "red" );
//    colors1.Insert_Element( "blue" );
//
//    colors2.Insert_Element( "blue" );
//    colors2.Insert_Element( "green" );
//    colors2.Insert_Element( "yellow" );
//
//    Math_Set<std::string> all_colors = UNION( colors1, colors2 );
//    std::cout << "Colors union: " << all_colors << std::endl;
//
//    TEST_ASSERT( all_colors.Set_Cardinality() == 4, "Color union has 4 elements" );
//}
//
// ============================================================================
// TEST 4: Set Intersection
// ============================================================================
//void test_intersection_operations()
//{
//    TEST_START( "Intersection Operations" );
//
//    TEST_SECTION( "Integer Intersection" );
//    Math_Set<int> X, Y;
//    X.Insert_Element( 1 );
//    X.Insert_Element( 2 );
//    X.Insert_Element( 3 );
//    X.Insert_Element( 4 );
//
//    Y.Insert_Element( 3 );
//    Y.Insert_Element( 4 );
//    Y.Insert_Element( 5 );
//    Y.Insert_Element( 6 );
//
//    Math_Set<int> Z = INTERSECTION( X, Y );
//
//    std::cout << "X = " << X << std::endl;
//    std::cout << "Y = " << Y << std::endl;
//    std::cout << "X ∩ Y = " << Z << std::endl;
//
//    TEST_ASSERT( Z.Set_Cardinality() == 2, "Intersection has 2 elements" );
//    TEST_ASSERT( Z [ 0 ] == 3 && Z [ 1 ] == 4, "Intersection is {3,4}" );
//
//    TEST_SECTION( "Disjoint Sets" );
//    Math_Set<int> disjoint1, disjoint2;
//    disjoint1.Insert_Element( 1 );
//    disjoint1.Insert_Element( 2 );
//
//    disjoint2.Insert_Element( 5 );
//    disjoint2.Insert_Element( 6 );
//
//    Math_Set<int> empty_intersection = INTERSECTION( disjoint1, disjoint2 );
//    TEST_ASSERT( empty_intersection.Is_Empty(), "Disjoint sets have empty intersection" );
//}
//
// ============================================================================
// TEST 5: Set Difference
// ============================================================================
//void test_difference_operations()
//{
//    TEST_START( "Difference Operations" );
//
//    Math_Set<int> P, Q;
//    P.Insert_Element( 1 );
//    P.Insert_Element( 2 );
//    P.Insert_Element( 3 );
//    P.Insert_Element( 4 );
//
//    Q.Insert_Element( 3 );
//    Q.Insert_Element( 4 );
//    Q.Insert_Element( 5 );
//
//    Math_Set<int> diff = DIFFERENCE( P, Q );
//
//    std::cout << "P = " << P << std::endl;
//    std::cout << "Q = " << Q << std::endl;
//    std::cout << "P - Q = " << diff << std::endl;
//
//    TEST_ASSERT( diff.Set_Cardinality() == 2, "P - Q has 2 elements" );
//    TEST_ASSERT( diff [ 0 ] == 1 && diff [ 1 ] == 2, "P - Q = {1,2}" );
//
//    TEST_SECTION( "Reverse Difference" );
//    Math_Set<int> diff2 = DIFFERENCE( Q, P );
//    std::cout << "Q - P = " << diff2 << std::endl;
//    TEST_ASSERT( diff2.Set_Cardinality() == 1, "Q - P has 1 element" );
//    TEST_ASSERT( diff2 [ 0 ] == 5, "Q - P = {5}" );
//}
//
// ============================================================================
// TEST 6: Symmetric Difference
// ============================================================================
//void test_symmetric_difference()
//{
//    TEST_START( "Symmetric Difference" );
//
//    Math_Set<int> M, N;
//    M.Insert_Element( 1 );
//    M.Insert_Element( 2 );
//    M.Insert_Element( 3 );
//
//    N.Insert_Element( 3 );
//    N.Insert_Element( 4 );
//    N.Insert_Element( 5 );
//
//    Math_Set<int> sym_diff = SYMMETRIC_DIFFERENCE( M, N );
//
//    std::cout << "M = " << M << std::endl;
//    std::cout << "N = " << N << std::endl;
//    std::cout << "M ⊕ N = " << sym_diff << std::endl;
//
//    TEST_ASSERT( sym_diff.Set_Cardinality() == 4, "Symmetric difference has 4 elements" );
//    TEST_ASSERT( sym_diff [ 0 ] == 1 && sym_diff [ 1 ] == 2 &&
//                 sym_diff [ 2 ] == 4 && sym_diff [ 3 ] == 5,
//                 "Symmetric difference is {1,2,4,5}" );
//}
//
// ============================================================================
// TEST 7: Subset Operations
// ============================================================================
//void test_subset_operations()
//{
//    TEST_START( "Subset Operations" );
//
//    Math_Set<int> super, sub, disjoint;
//
//    super.Insert_Element( 1 );
//    super.Insert_Element( 2 );
//    super.Insert_Element( 3 );
//    super.Insert_Element( 4 );
//    super.Insert_Element( 5 );
//
//    sub.Insert_Element( 2 );
//    sub.Insert_Element( 4 );
//
//    disjoint.Insert_Element( 10 );
//    disjoint.Insert_Element( 20 );
//
//    std::cout << "Super-Set = " << super << std::endl;
//    std::cout << "Subset = " << sub << std::endl;
//    std::cout << "Disjoint = " << disjoint << std::endl;
//
//    TEST_SECTION( "Subset Tests" );
//    TEST_ASSERT( IS_SUBSET( sub, super ), "sub ⊆ super" );
//    TEST_ASSERT( !IS_SUBSET( super, sub ), "super ⊄ sub" );
//    TEST_ASSERT( !IS_SUBSET( disjoint, super ), "disjoint ⊄ super" );
//
//    TEST_SECTION( "Proper Subset Tests" );
//    TEST_ASSERT( IS_PROPER_SUBSET( sub, super ), "sub ⊂ super (proper)" );
//    TEST_ASSERT( !IS_PROPER_SUBSET( super, super ), "super ⊄ super (not proper)" );
//
//    TEST_SECTION( "Self Subset" );
//    TEST_ASSERT( IS_SUBSET( super, super ), "Every set is subset of itself" );
//}
//
// ============================================================================
// TEST 8: Cartesian Product
// ============================================================================
//void test_cartesian_product()
//{
//    TEST_START( "Cartesian Product" );
//
//    TEST_SECTION( "Integer × Integer" );
//    Math_Set<int> A, B;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//
//    B.Insert_Element( 3 );
//    B.Insert_Element( 4 );
//
//    Math_Set<std::pair<int, int>> AxB = CARTESIAN_PRODUCT( A, B );
//
//    std::cout << "A = " << A << std::endl;
//    std::cout << "B = " << B << std::endl;
//    std::cout << "A × B = ";
//    for ( int i = 0; i < AxB.Get_Size(); i++ )
//    {
//        std::cout << "(" << AxB [ i ].first << "," << AxB [ i ].second << ") ";
//    }
//    std::cout << std::endl;
//
//    TEST_ASSERT( AxB.Set_Cardinality() == 4, "A × B has 4 pairs" );
//    TEST_ASSERT( AxB [ 0 ].first == 1 && AxB [ 0 ].second == 3, "First pair is (1,3)" );
//
//    TEST_SECTION( "String × Integer" );
//    Math_Set<std::string> names;
//    Math_Set<int> ids;
//
//    names.Insert_Element( "Alice" );
//    names.Insert_Element( "Bob" );
//
//    ids.Insert_Element( 100 );
//    ids.Insert_Element( 200 );
//
//    Math_Set<std::pair<std::string, int>> nameIds = CARTESIAN_PRODUCT( names, ids );
//
//    std::cout << "Names × IDs = ";
//    for ( int i = 0; i < nameIds.Get_Size(); i++ )
//    {
//        std::cout << "(" << nameIds [ i ].first << "," << nameIds [ i ].second << ") ";
//    }
//    std::cout << std::endl;
//
//    TEST_ASSERT( nameIds.Set_Cardinality() == 4, "Names × IDs has 4 pairs" );
//}
//
// ============================================================================
// TEST 9: Power Set
// ============================================================================
//void test_power_set()
//{
//    TEST_START( "Power Set" );
//
//    TEST_SECTION( "Small Set" );
//    Math_Set<int> small;
//    small.Insert_Element( 1 );
//    small.Insert_Element( 2 );
//    small.Insert_Element( 3 );
//
//    Math_Set<Math_Set<int>> power = POWER_SET( small );
//
//    std::cout << "Original set: " << small << std::endl;
//    std::cout << "Power set (2^" << small.Set_Cardinality() << " = "
//        << power.Set_Cardinality() << " subsets):" << std::endl;
//
//    for ( int i = 0; i < power.Get_Size(); i++ )
//    {
//        std::cout << "  " << power [ i ] << std::endl;
//    }
//
//    TEST_ASSERT( power.Set_Cardinality() == 8, "Power set has 2^3 = 8 subsets" );
//    TEST_ASSERT( power [ 0 ].Is_Empty(), "First subset is empty set" );
//
//    TEST_SECTION( "Empty Set" );
//    Math_Set<int> empty;
//    Math_Set<Math_Set<int>> power_empty = POWER_SET( empty );
//    TEST_ASSERT( power_empty.Set_Cardinality() == 1, "Power set of empty set has 1 element (empty set)" );
//
//    TEST_SECTION( "Singleton Set" );
//    Math_Set<int> singleton;
//    singleton.Insert_Element( 42 );
//    Math_Set<Math_Set<int>> power_singleton = POWER_SET( singleton );
//    TEST_ASSERT( power_singleton.Set_Cardinality() == 2, "Power set of singleton has 2 elements" );
//}
//
// ============================================================================
// TEST 10: Copy and Assignment
// ============================================================================
//void test_copy_and_assignment()
//{
//    TEST_START( "Copy Constructor and Assignment" );
//
//    TEST_SECTION( "Copy Constructor" );
//    Math_Set<int> original;
//    original.Insert_Element( 10 );
//    original.Insert_Element( 20 );
//    original.Insert_Element( 30 );
//
//    Math_Set<int> copy( original );
//
//    TEST_ASSERT( copy.Set_Cardinality() == 3, "Copy has same size" );
//    TEST_ASSERT( copy == original, "Copy equals original" );
//
//     Modify copy
//    copy.Insert_Element( 40 );
//    TEST_ASSERT( copy != original, "Modified copy differs from original" );
//    TEST_ASSERT( original.Set_Cardinality() == 3, "Original unchanged" );
//
//    TEST_SECTION( "Assignment Operator" );
//    Math_Set<int> assigned;
//    assigned.Insert_Element( 99 );
//
//    assigned = original;
//    TEST_ASSERT( assigned == original, "Assigned set equals original" );
//    TEST_ASSERT( assigned.Set_Cardinality() == 3, "Assigned set has correct size" );
//}
//
// ============================================================================
// TEST 11: Edge Cases
// ============================================================================
//void test_edge_cases()
//{
//    TEST_START( "Edge Cases" );
//
//    TEST_SECTION( "Large Set" );
//    Math_Set<int> large;
//    for ( int i = 100; i >= 1; i-- )
//    {
//        large.Insert_Element( i );
//    }
//    TEST_ASSERT( large.Set_Cardinality() == 100, "Large set has 100 elements" );
//    TEST_ASSERT( large [ 0 ] == 1, "Elements are sorted (first is 1)" );
//    TEST_ASSERT( large [ 99 ] == 100, "Elements are sorted (last is 100)" );
//
//    TEST_SECTION( "Capacity Growth" );
//    Math_Set<int> growing( 2 ); // Start with small capacity
//    for ( int i = 1; i <= 20; i++ )
//    {
//        growing.Insert_Element( i );
//    }
//    TEST_ASSERT( growing.Set_Cardinality() == 20, "Set grew to accommodate 20 elements" );
//    TEST_ASSERT( growing.Get_Capacity() >= 20, "Capacity increased automatically" );
//
//    TEST_SECTION( "Negative Numbers" );
//    Math_Set<int> negatives;
//    negatives.Insert_Element( -5 );
//    negatives.Insert_Element( -1 );
//    negatives.Insert_Element( -10 );
//    negatives.Insert_Element( 0 );
//    negatives.Insert_Element( 5 );
//
//    std::cout << "Set with negatives: " << negatives << std::endl;
//    TEST_ASSERT( negatives [ 0 ] == -10, "Negative numbers sorted correctly" );
//    TEST_ASSERT( negatives [ 4 ] == 5, "Positive numbers sorted correctly" );
//}
//
// ============================================================================
// TEST 12: Stress Test
// ============================================================================
//void test_stress_operations()
//{
//    TEST_START( "Stress Tests" );
//
//    TEST_SECTION( "Many Insertions and Removals" );
//    Math_Set<int> stress;
//
//     Insert 1000 elements
//    for ( int i = 0; i < 1000; i++ )
//    {
//        stress.Insert_Element( i );
//    }
//    TEST_ASSERT( stress.Set_Cardinality() == 1000, "1000 elements inserted" );
//
//     Remove every other element
//    for ( int i = 0; i < 1000; i += 2 )
//    {
//        stress.Remove_Element( i );
//    }
//    TEST_ASSERT( stress.Set_Cardinality() == 500, "500 elements remain after removal" );
//
//     Verify remaining elements
//    bool correct = true;
//    for ( int i = 0; i < 500; i++ )
//    {
//        if ( stress [ i ] != i * 2 + 1 )
//        {
//            correct = false;
//            break;
//        }
//    }
//    TEST_ASSERT( correct, "Remaining elements are odd numbers in order" );
//
//    TEST_SECTION( "Large Union" );
//    Math_Set<int> set1, set2;
//    for ( int i = 0; i < 500; i++ )
//    {
//        set1.Insert_Element( i );
//        set2.Insert_Element( i + 250 );
//    }
//
//    Math_Set<int> large_union = UNION( set1, set2 );
//    TEST_ASSERT( large_union.Set_Cardinality() == 750, "Large union has 750 elements" );
//}
//
// ============================================================================
// TEST 13: String Edge Cases
// ============================================================================
//void test_string_edge_cases()
//{
//    TEST_START( "String Edge Cases" );
//
//    TEST_SECTION( "Empty Strings" );
//    Math_Set<std::string> strings;
//    strings.Insert_Element( "" );
//    strings.Insert_Element( "a" );
//    strings.Insert_Element( "aa" );
//
//    std::cout << "Strings with empty: " << strings << std::endl;
//    TEST_ASSERT( strings [ 0 ] == "", "Empty string sorts first" );
//
//    TEST_SECTION( "Long Strings" );
//    Math_Set<std::string> long_strings;
//    std::string long1( 100, 'a' );
//    std::string long2( 100, 'b' );
//
//    long_strings.Insert_Element( long1 );
//    long_strings.Insert_Element( long2 );
//
//    TEST_ASSERT( long_strings.Set_Cardinality() == 2, "Long strings handled correctly" );
//
//    TEST_SECTION( "Special Characters" );
//    Math_Set<std::string> special;
//    special.Insert_Element( "hello!" );
//    special.Insert_Element( "world?" );
//    special.Insert_Element( "test#123" );
//
//    TEST_ASSERT( special.Set_Cardinality() == 3, "Special characters handled" );
//}
//
// ============================================================================
// TEST 14: Comparison Operators
// ============================================================================
//void test_comparison_operators()
//{
//    TEST_START( "Comparison Operators" );
//
//    Math_Set<int> set1, set2, set3;
//
//    set1.Insert_Element( 1 );
//    set1.Insert_Element( 2 );
//    set1.Insert_Element( 3 );
//
//    set2.Insert_Element( 1 );
//    set2.Insert_Element( 2 );
//    set2.Insert_Element( 3 );
//
//    set3.Insert_Element( 1 );
//    set3.Insert_Element( 2 );
//
//    TEST_SECTION( "Equality" );
//    TEST_ASSERT( set1 == set2, "Identical sets are equal" );
//    TEST_ASSERT( !( set1 == set3 ), "Different sets are not equal" );
//
//    TEST_SECTION( "Inequality" );
//    TEST_ASSERT( set1 != set3, "Different sets are not equal" );
//    TEST_ASSERT( !( set1 != set2 ), "Identical sets are equal" );
//
//    TEST_SECTION( "Less Than (for nested sets)" );
//    TEST_ASSERT( set3 < set1, "Smaller set is less than larger set" );
//    TEST_ASSERT( !( set1 < set2 ), "Equal sets are not less than each other" );
//}
//
// ============================================================================
// MAIN TEST RUNNER
// ============================================================================
//int main()
//{
//    std::cout << BOLD << BLUE;
//    std::cout << "\n";
//    std::cout << "╔════════════════════════════════════════════════╗\n";
//    std::cout << "║                                                ║\n";
//    std::cout << "║     COMPREHENSIVE MATH_SET TEST SUITE         ║\n";
//    std::cout << "║                                                ║\n";
//    std::cout << "╚════════════════════════════════════════════════╝\n";
//    std::cout << RESET << std::endl;
//
//    try
//    {
//    Run all tests
//        test_basic_integer_operations();
//        test_string_operations();
//        test_union_operations();
//        test_intersection_operations();
//        test_difference_operations();
//        test_symmetric_difference();
//        test_subset_operations();
//        test_cartesian_product();
//        test_power_set();
//        test_copy_and_assignment();
//        test_edge_cases();
//        test_stress_operations();
//        test_string_edge_cases();
//        test_comparison_operators();
//
//         Print summary
//        print_summary();
//
//        return ( failed_tests == 0 ) ? 0 : 1;
//
//    }
//    catch ( const std::exception & e )
//    {
//        std::cout << RED << BOLD << "\n💥 EXCEPTION CAUGHT: " << e.what() << RESET << std::endl;
//        return 1;
//    }
//}
//
// Quantifier Tests
//
//#include "Quantifier_Validator.h"
//
// ANSI color codes for pretty output
//#define RESET   "\033[0m"
//#define RED     "\033[31m"
//#define GREEN   "\033[32m"
//#define YELLOW  "\033[33m"
//#define BLUE    "\033[34m"
//#define MAGENTA "\033[35m"
//#define CYAN    "\033[36m"
//#define BOLD    "\033[1m"
//
// Test result tracking
//int total_tests = 0;
//int passed_tests = 0;
//int failed_tests = 0;
//
// Test helper macros
//#define TEST_START(name) \
//    std::cout << CYAN << "\n=== Testing: " << name << " ===" << RESET << std::endl; \
//    total_tests++;
//
//#define TEST_ASSERT(condition, message) \
//    if (condition) { \
//        std::cout << GREEN << "✓ PASS: " << message << RESET << std::endl; \
//        passed_tests++; \
//    } else { \
//        std::cout << RED << "✗ FAIL: " << message << RESET << std::endl; \
//        failed_tests++; \
//    }
//
//#define TEST_SECTION(name) \
//    std::cout << YELLOW << "\n--- " << name << " ---" << RESET << std::endl;
//
// Print test summary
//void print_summary()
//{
//    std::cout << BOLD << "\n" << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << BOLD << "TEST SUMMARY" << RESET << std::endl;
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << "Total Tests: " << total_tests << std::endl;
//    std::cout << GREEN << "Passed: " << passed_tests << RESET << std::endl;
//    std::cout << RED << "Failed: " << failed_tests << RESET << std::endl;
//
//    if ( failed_tests == 0 )
//    {
//        std::cout << GREEN << BOLD << "\n🎉 ALL TESTS PASSED! 🎉" << RESET << std::endl;
//    }
//    else
//    {
//        std::cout << RED << BOLD << "\n⚠️  SOME TESTS FAILED ⚠️" << RESET << std::endl;
//    }
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << "\n" << std::endl;
//}
//
// ============================================================================
// TEST 1: Universal Quantifier - Integer Predicates
// ============================================================================
//void test_universal_quantifier_integers()
//{
//    TEST_START( "Universal Quantifier - Integers" );
//
//    Math_Set<int> numbers;
//    numbers.Insert_Element( 2 );
//    numbers.Insert_Element( 4 );
//    numbers.Insert_Element( 6 );
//    numbers.Insert_Element( 8 );
//
//    std::cout << "Set: " << numbers << std::endl;
//
//    TEST_SECTION( "All Even Numbers" );
//    auto is_even = [] ( int x )
//    {
//        return x % 2 == 0;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, is_even );
//    TEST_ASSERT( result == true, "∀x: x is even (should be true)" );
//
//    TEST_SECTION( "All Greater Than Zero" );
//    auto is_positive = [] ( int x )
//    {
//        return x > 0;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, is_positive );
//    TEST_ASSERT( result == true, "∀x: x > 0 (should be true)" );
//
//    TEST_SECTION( "All Greater Than 5 (should fail)" );
//    auto greater_than_5 = [] ( int x )
//    {
//        return x > 5;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, greater_than_5 );
//    TEST_ASSERT( result == false, "∀x: x > 5 (should be false, 2 and 4 fail)" );
//
//    TEST_SECTION( "All Odd (should fail)" );
//    auto is_odd = [] ( int x )
//    {
//        return x % 2 == 1;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, is_odd );
//    TEST_ASSERT( result == false, "∀x: x is odd (should be false)" );
//
//    TEST_SECTION( "Empty Set (vacuous truth)" );
//    Math_Set<int> empty;
//    auto always_false = [] ( int x )
//    {
//        return false;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( empty, always_false );
//    TEST_ASSERT( result == true, "∀x ∈ ∅: false (vacuously true)" );
//}
//
// ============================================================================
// TEST 2: Universal Quantifier - String Predicates
// ============================================================================
//void test_universal_quantifier_strings()
//{
//    TEST_START( "Universal Quantifier - Strings" );
//
//    Math_Set<std::string> words;
//    words.Insert_Element( "apple" );
//    words.Insert_Element( "apricot" );
//    words.Insert_Element( "avocado" );
//
//    std::cout << "Set: " << words << std::endl;
//
//    TEST_SECTION( "All Start with 'a'" );
//    auto starts_with_a = [] ( const std::string & s )
//    {
//        return !s.empty() && s [ 0 ] == 'a';
//    };
//    bool result = Single_Quantifier_Validator<std::string>::Universal_Quantifier( words, starts_with_a );
//    TEST_ASSERT( result == true, "∀word: starts with 'a' (should be true)" );
//
//    TEST_SECTION( "All Length > 4" );
//    auto length_gt_4 = [] ( const std::string & s )
//    {
//        return s.length() > 4;
//    };
//    result = Single_Quantifier_Validator<std::string>::Universal_Quantifier( words, length_gt_4 );
//    TEST_ASSERT( result == true, "∀word: length > 4 (should be true)" );
//
//    TEST_SECTION( "All Contains 'p'" );
//    auto contains_p = [] ( const std::string & s )
//    {
//        return s.find( 'p' ) != std::string::npos;
//    };
//    result = Single_Quantifier_Validator<std::string>::Universal_Quantifier( words, contains_p );
//    TEST_ASSERT( result == false, "∀word: contains 'p' (should be false, 'avocado' doesn't)" );
//}
//
// ============================================================================
// TEST 3: Existential Quantifier - Integer Predicates
// ============================================================================
//void test_existential_quantifier_integers()
//{
//    TEST_START( "Existential Quantifier - Integers" );
//
//    Math_Set<int> numbers;
//    numbers.Insert_Element( 1 );
//    numbers.Insert_Element( 3 );
//    numbers.Insert_Element( 5 );
//    numbers.Insert_Element( 8 );
//
//    std::cout << "Set: " << numbers << std::endl;
//
//    TEST_SECTION( "Exists Even Number" );
//    auto is_even = [] ( int x )
//    {
//        return x % 2 == 0;
//    };
//    bool result = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, is_even );
//    TEST_ASSERT( result == true, "∃x: x is even (8 exists)" );
//
//    TEST_SECTION( "Exists Greater Than 7" );
//    auto gt_7 = [] ( int x )
//    {
//        return x > 7;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, gt_7 );
//    TEST_ASSERT( result == true, "∃x: x > 7 (8 exists)" );
//
//    TEST_SECTION( "Exists Negative (should fail)" );
//    auto is_negative = [] ( int x )
//    {
//        return x < 0;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, is_negative );
//    TEST_ASSERT( result == false, "∃x: x < 0 (should be false)" );
//
//    TEST_SECTION( "Exists Prime" );
//    auto is_prime = [] ( int n )
//    {
//        if ( n <= 1 ) return false;
//        if ( n <= 3 ) return true;
//        if ( n % 2 == 0 || n % 3 == 0 ) return false;
//        for ( int i = 5; i * i <= n; i += 6 )
//        {
//            if ( n % i == 0 || n % ( i + 2 ) == 0 ) return false;
//        }
//        return true;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, is_prime );
//    TEST_ASSERT( result == true, "∃x: x is prime (1, 3, 5 are prime)" );
//}
//
// ============================================================================
// TEST 4: Existential Quantifier - String Predicates
// ============================================================================
//void test_existential_quantifier_strings()
//{
//    TEST_START( "Existential Quantifier - Strings" );
//
//    Math_Set<std::string> fruits;
//    fruits.Insert_Element( "apple" );
//    fruits.Insert_Element( "banana" );
//    fruits.Insert_Element( "cherry" );
//
//    std::cout << "Set: " << fruits << std::endl;
//
//    TEST_SECTION( "Exists Starting with 'b'" );
//    auto starts_with_b = [] ( const std::string & s )
//    {
//        return !s.empty() && s [ 0 ] == 'b';
//    };
//    bool result = Single_Quantifier_Validator<std::string>::Existential_Quantifier( fruits, starts_with_b );
//    TEST_ASSERT( result == true, "∃word: starts with 'b' (banana)" );
//
//    TEST_SECTION( "Exists Length > 6" );
//    auto length_gt_6 = [] ( const std::string & s )
//    {
//        return s.length() > 6;
//    };
//    result = Single_Quantifier_Validator<std::string>::Existential_Quantifier( fruits, length_gt_6 );
//    TEST_ASSERT( result == false, "∃word: length > 6 (should be false)" );
//
//    TEST_SECTION( "Exists Contains 'rr'" );
//    auto contains_rr = [] ( const std::string & s )
//    {
//        return s.find( "rr" ) != std::string::npos;
//    };
//    result = Single_Quantifier_Validator<std::string>::Existential_Quantifier( fruits, contains_rr );
//    TEST_ASSERT( result == true, "∃word: contains 'rr' (cherry)" );
//}
//
// ============================================================================
// TEST 5: Unique Existential Quantifier
// ============================================================================
//void test_unique_existential_quantifier()
//{
//    TEST_START( "Unique Existential Quantifier" );
//
//    TEST_SECTION( "Exactly One Even Number" );
//    Math_Set<int> numbers1;
//    numbers1.Insert_Element( 1 );
//    numbers1.Insert_Element( 3 );
//    numbers1.Insert_Element( 4 );
//    numbers1.Insert_Element( 7 );
//
//    std::cout << "Set: " << numbers1 << std::endl;
//
//    auto is_even = [] ( int x )
//    {
//        return x % 2 == 0;
//    };
//    bool result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( numbers1, is_even );
//    TEST_ASSERT( result == true, "∃!x: x is even (only 4)" );
//
//    TEST_SECTION( "Multiple Even Numbers (should fail)" );
//    Math_Set<int> numbers2;
//    numbers2.Insert_Element( 2 );
//    numbers2.Insert_Element( 4 );
//    numbers2.Insert_Element( 6 );
//
//    std::cout << "Set: " << numbers2 << std::endl;
//
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( numbers2, is_even );
//    TEST_ASSERT( result == false, "∃!x: x is even (multiple exist)" );
//
//    TEST_SECTION( "No Even Numbers (should fail)" );
//    Math_Set<int> numbers3;
//    numbers3.Insert_Element( 1 );
//    numbers3.Insert_Element( 3 );
//    numbers3.Insert_Element( 5 );
//
//    std::cout << "Set: " << numbers3 << std::endl;
//
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( numbers3, is_even );
//    TEST_ASSERT( result == false, "∃!x: x is even (none exist)" );
//
//    TEST_SECTION( "Unique String Length" );
//    Math_Set<std::string> words;
//    words.Insert_Element( "cat" );
//    words.Insert_Element( "dog" );
//    words.Insert_Element( "elephant" );
//
//    std::cout << "Set: " << words << std::endl;
//
//    auto length_8 = [] ( const std::string & s )
//    {
//        return s.length() == 8;
//    };
//    result = Single_Quantifier_Validator<std::string>::Unique_Existential_Quantifier( words, length_8 );
//    TEST_ASSERT( result == true, "∃!word: length == 8 (only 'elephant')" );
//}
//
// ============================================================================
// TEST 6: De Morgan's Laws
// ============================================================================
//void test_demorgan_laws()
//{
//    TEST_START( "De Morgan's Laws for Quantifiers" );
//
//    Math_Set<int> numbers;
//    numbers.Insert_Element( 2 );
//    numbers.Insert_Element( 4 );
//    numbers.Insert_Element( 5 );
//    numbers.Insert_Element( 8 );
//
//    std::cout << "Set: " << numbers << std::endl;
//
//    TEST_SECTION( "Universal De Morgan: ¬(∃x P(x)) ≡ ∀x ¬P(x)" );
//    auto all_even = [] ( int x )
//    {
//        return x % 2 == 0;
//    };
//
// ¬(∃x P(x)) - not exists even
//    bool not_exists_even = !Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, all_even );
//
//     ∀x ¬P(x) - all are not even (all are odd)
//    auto not_even = [] ( int x )
//    {
//        return x % 2 != 0;
//    };
//    bool all_not_even = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, not_even );
//
//    TEST_ASSERT( not_exists_even == all_not_even,
//                 "¬(∃x: even) ≡ ∀x: ¬even (both should be false)" );
//
//    TEST_SECTION( "Existential De Morgan: ¬(∀x P(x)) ≡ ∃x ¬P(x)" );
//     ¬(∀x P(x)) - not all are even
//    bool not_all_even = !Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, all_even );
//
//     ∃x ¬P(x) - exists at least one not even
//    bool exists_not_even = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, not_even );
//
//    TEST_ASSERT( not_all_even == exists_not_even,
//                 "¬(∀x: even) ≡ ∃x: ¬even (both should be true, 5 is odd)" );
//
//    TEST_SECTION( "String De Morgan" );
//    Math_Set<std::string> words;
//    words.Insert_Element( "hello" );
//    words.Insert_Element( "world" );
//    words.Insert_Element( "test" );
//
//    std::cout << "Set: " << words << std::endl;
//
//    auto has_o = [] ( const std::string & s )
//    {
//        return s.find( 'o' ) != std::string::npos;
//    };
//    auto no_o = [] ( const std::string & s )
//    {
//        return s.find( 'o' ) == std::string::npos;
//    };
//
//    bool not_all_have_o = !Single_Quantifier_Validator<std::string>::Universal_Quantifier( words, has_o );
//    bool exists_no_o = Single_Quantifier_Validator<std::string>::Existential_Quantifier( words, no_o );
//
//    TEST_ASSERT( not_all_have_o == exists_no_o,
//                 "¬(∀word: has 'o') ≡ ∃word: no 'o' (both true, 'test' has no 'o')" );
//}
//
// ============================================================================
// TEST 7: Nested Quantifiers - ∀∀
// ============================================================================
//void test_nested_universal_universal()
//{
//    TEST_START( "Nested Quantifiers: ∀x∀y" );
//
//    Math_Set<int> A, B;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//
//    B.Insert_Element( 10 );
//    B.Insert_Element( 20 );
//
//    std::cout << "Set A: " << A << std::endl;
//    std::cout << "Set B: " << B << std::endl;
//
//    TEST_SECTION( "∀x∈A ∀y∈B: x < y" );
//    auto less_than = [] ( int x, int y )
//    {
//        return x < y;
//    };
//    bool result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal( A, B, less_than );
//    TEST_ASSERT( result == true, "All pairs (x,y) satisfy x < y" );
//
//    TEST_SECTION( "∀x∈A ∀y∈B: x + y > 5" );
//    auto sum_gt_5 = [] ( int x, int y )
//    {
//        return x + y > 5;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal( A, B, sum_gt_5 );
//    TEST_ASSERT( result == true, "All pairs satisfy x + y > 5" );
//
//    TEST_SECTION( "∀x∈A ∀y∈B: x > y (should fail)" );
//    auto greater_than = [] ( int x, int y )
//    {
//        return x > y;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal( A, B, greater_than );
//    TEST_ASSERT( result == false, "Not all pairs satisfy x > y" );
//
//    TEST_SECTION( "String × String: All lengths compatible" );
//    Math_Set<std::string> words1, words2;
//    words1.Insert_Element( "hi" );
//    words1.Insert_Element( "ok" );
//
//    words2.Insert_Element( "test" );
//    words2.Insert_Element( "data" );
//
//    std::cout << "Words1: " << words1 << std::endl;
//    std::cout << "Words2: " << words2 << std::endl;
//
//    auto first_shorter = [] ( const std::string & a, const std::string & b )
//    {
//        return a.length() < b.length();
//    };
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Universal_Universal(
//        words1, words2, first_shorter );
//    TEST_ASSERT( result == true, "∀a∈words1 ∀b∈words2: len(a) < len(b)" );
//}
//
// ============================================================================
// TEST 8: Nested Quantifiers - ∀∃
// ============================================================================
//void test_nested_universal_existential()
//{
//    TEST_START( "Nested Quantifiers: ∀x∃y" );
//
//    Math_Set<int> A, B;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//    A.Insert_Element( 3 );
//
//    B.Insert_Element( 2 );
//    B.Insert_Element( 4 );
//    B.Insert_Element( 6 );
//
//    std::cout << "Set A: " << A << std::endl;
//    std::cout << "Set B: " << B << std::endl;
//
//    TEST_SECTION( "∀x∈A ∃y∈B: y = 2x" );
//    auto double_relation = [] ( int x, int y )
//    {
//        return y == 2 * x;
//    };
//    bool result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Existential( A, B, double_relation );
//    TEST_ASSERT( result == true, "For each x, there exists y = 2x" );
//
//    TEST_SECTION( "∀x∈A ∃y∈B: y > x" );
//    auto greater = [] ( int x, int y )
//    {
//        return y > x;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Existential( A, B, greater );
//    TEST_ASSERT( result == true, "For each x, there exists y > x" );
//
//    TEST_SECTION( "∀x∈A ∃y∈B: y < x (should fail)" );
//    auto less = [] ( int x, int y )
//    {
//        return y < x;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Existential( A, B, less );
//    TEST_ASSERT( result == false, "Not for all x exists y < x (1 has no smaller y)" );
//
//    TEST_SECTION( "String: Every name has longer word" );
//    Math_Set<std::string> names, words;
//    names.Insert_Element( "Jo" );
//    names.Insert_Element( "Sam" );
//
//    words.Insert_Element( "test" );
//    words.Insert_Element( "hello" );
//    words.Insert_Element( "world" );
//
//    std::cout << "Names: " << names << std::endl;
//    std::cout << "Words: " << words << std::endl;
//
//    auto name_shorter_than_word = [] ( const std::string & name, const std::string & word )
//    {
//        return name.length() < word.length();
//    };
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Universal_Existential(
//        names, words, name_shorter_than_word );
//    TEST_ASSERT( result == true, "∀name ∃word: len(name) < len(word)" );
//}
//
// ============================================================================
// TEST 9: Nested Quantifiers - ∃∃
// ============================================================================
//void test_nested_existential_existential()
//{
//    TEST_START( "Nested Quantifiers: ∃x∃y" );
//
//    Math_Set<int> A, B;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 3 );
//    A.Insert_Element( 5 );
//
//    B.Insert_Element( 2 );
//    B.Insert_Element( 4 );
//    B.Insert_Element( 6 );
//    B.Insert_Element( 9 );  // Add 9 to make 1+9=10 work
//
//    std::cout << "Set A: " << A << std::endl;
//    std::cout << "Set B: " << B << std::endl;
//
//    TEST_SECTION( "∃x∈A ∃y∈B: x + y = 10" );
//    auto sum_10 = [] ( int x, int y )
//    {
//        return x + y == 10;
//    };
//    bool result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Existential( A, B, sum_10 );
//    TEST_ASSERT( result == true, "∃ pair (x,y): x + y = 10 (e.g., 4+6)" );
//
//    TEST_SECTION( "∃x∈A ∃y∈B: x > y" );
//    auto x_greater = [] ( int x, int y )
//    {
//        return x > y;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Existential( A, B, x_greater );
//    TEST_ASSERT( result == true, "∃ pair where x > y (e.g., 5 > 2)" );
//
//    TEST_SECTION( "∃x∈A ∃y∈B: x * y > 100 (should fail)" );
//    auto product_gt_100 = [] ( int x, int y )
//    {
//        return x * y > 100;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Existential( A, B, product_gt_100 );
//    TEST_ASSERT( result == false, "No pair satisfies x * y > 100" );
//
//    TEST_SECTION( "String: Exists pair with common character" );
//    Math_Set<std::string> set1, set2;
//    set1.Insert_Element( "apple" );
//    set1.Insert_Element( "banana" );
//
//    set2.Insert_Element( "cat" );
//    set2.Insert_Element( "dog" );
//
//    std::cout << "Set1: " << set1 << std::endl;
//    std::cout << "Set2: " << set2 << std::endl;
//
//    auto has_common_char = [] ( const std::string & s1, const std::string & s2 )
//    {
//        for ( char c : s1 )
//        {
//            if ( s2.find( c ) != std::string::npos ) return true;
//        }
//        return false;
//    };
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Existential_Existential(
//        set1, set2, has_common_char );
//    TEST_ASSERT( result == true, "∃ pair with common character (e.g., 'banana' and 'cat' share 'a')" );
//}
//
// ============================================================================
// TEST 10: Nested Quantifiers - ∃∀
// ============================================================================
//void test_nested_existential_universal()
//{
//    TEST_START( "Nested Quantifiers: ∃x∀y" );
//
//    Math_Set<int> A, B;
//    A.Insert_Element( 10 );
//    A.Insert_Element( 20 );
//    A.Insert_Element( 30 );
//
//    B.Insert_Element( 1 );
//    B.Insert_Element( 2 );
//    B.Insert_Element( 3 );
//
//    std::cout << "Set A: " << A << std::endl;
//    std::cout << "Set B: " << B << std::endl;
//
//    TEST_SECTION( "∃x∈A ∀y∈B: x > y" );
//    auto x_greater = [] ( int x, int y )
//    {
//        return x > y;
//    };
//    bool result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Universal( A, B, x_greater );
//    TEST_ASSERT( result == true, "∃x that is greater than all y (e.g., 10 > all in B)" );
//
//    TEST_SECTION( "∃x∈A ∀y∈B: x = y (should fail)" );
//    auto equal = [] ( int x, int y )
//    {
//        return x == y;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Universal( A, B, equal );
//    TEST_ASSERT( result == false, "No x equals all y" );
//
//    TEST_SECTION( "∃x∈A ∀y∈B: x % y == 0" );
//    auto divisible = [] ( int x, int y )
//    {
//        return x % y == 0;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Universal( A, B, divisible );
//    TEST_ASSERT( result == true, "∃x divisible by all y (30 is divisible by 1,2,3)" );
//
//    TEST_SECTION( "String: Exists word longer than all short words" );
//    Math_Set<std::string> long_words, short_words;
//    long_words.Insert_Element( "hi" );
//    long_words.Insert_Element( "test" );
//    long_words.Insert_Element( "hello" );
//
//    short_words.Insert_Element( "a" );
//    short_words.Insert_Element( "I" );
//
//    std::cout << "Long words: " << long_words << std::endl;
//    std::cout << "Short words: " << short_words << std::endl;
//
//    auto longer_than_all = [] ( const std::string & long_word, const std::string & short_word )
//    {
//        return long_word.length() > short_word.length();
//    };
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Existential_Universal(
//        long_words, short_words, longer_than_all );
//    TEST_ASSERT( result == true, "∃ long word that is longer than all short words" );
//}
//
// ============================================================================
// TEST 11: Edge Cases with Empty Sets
// ============================================================================
//void test_empty_set_edge_cases()
//{
//    TEST_START( "Edge Cases with Empty Sets" );
//
//    Math_Set<int> empty;
//    Math_Set<int> non_empty;
//    non_empty.Insert_Element( 5 );
//
//    TEST_SECTION( "Universal on Empty Set (vacuous truth)" );
//    auto always_false = [] ( int x )
//    {
//        return false;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( empty, always_false );
//    TEST_ASSERT( result == true, "∀x∈∅: false is vacuously true" );
//
//    TEST_SECTION( "Existential on Empty Set" );
//    auto always_true = [] ( int x )
//    {
//        return true;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( empty, always_true );
//    TEST_ASSERT( result == false, "∃x∈∅: true is false (no elements)" );
//
//    TEST_SECTION( "Unique Existential on Empty Set" );
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( empty, always_true );
//    TEST_ASSERT( result == false, "∃!x∈∅: true is false" );
//
//    TEST_SECTION( "Nested with Empty Sets" );
//    auto any_predicate = [] ( int x, int y )
//    {
//        return true;
//    };
//
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal(
//        empty, non_empty, any_predicate );
//    TEST_ASSERT( result == true, "∀x∈∅ ∀y: P(x,y) is vacuously true" );
//
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Existential(
//        empty, non_empty, any_predicate );
//    TEST_ASSERT( result == false, "∃x∈∅ ∃y: P(x,y) is false" );
//}
//
// ============================================================================
// TEST 12: Complex Mathematical Predicates
// ============================================================================
//void test_complex_mathematical_predicates()
//{
//    TEST_START( "Complex Mathematical Predicates" );
//
//    Math_Set<int> numbers;
//    numbers.Insert_Element( 2 );
//    numbers.Insert_Element( 3 );
//    numbers.Insert_Element( 4 );
//    numbers.Insert_Element( 9 );
//    numbers.Insert_Element( 16 );
//
//    std::cout << "Set: " << numbers << std::endl;
//
//    TEST_SECTION( "Perfect Squares" );
//    auto is_perfect_square = [] ( int x )
//    {
//        int root = static_cast< int >( std::sqrt( x ) );
//        return root * root == x;
//    };
//    bool result = Single_Quantifier_Validator<int>::Existential_Quantifier( numbers, is_perfect_square );
//    TEST_ASSERT( result == true, "∃x: x is perfect square (4, 9, 16)" );
//
//    TEST_SECTION( "All Powers of 2" );
//    auto is_power_of_2 = [] ( int x )
//    {
//        return x > 0 && ( x & ( x - 1 ) ) == 0;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( numbers, is_power_of_2 );
//    TEST_ASSERT( result == false, "Not all are powers of 2 (3, 9 aren't)" );
//
//    TEST_SECTION( "Unique Perfect Cube" );
//    Math_Set<int> cubes;
//    cubes.Insert_Element( 1 );
//    cubes.Insert_Element( 8 );
//    cubes.Insert_Element( 10 );
//    cubes.Insert_Element( 27 );
//
//    std::cout << "Cubes set: " << cubes << std::endl;
//
//    auto is_perfect_cube = [] ( int x )
//    {
//        int root = static_cast< int >( std::cbrt( x ) + 0.5 );
//        return root * root * root == x;
//    };
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( cubes, is_perfect_cube );
//    TEST_ASSERT( result == false, "Multiple perfect cubes exist (1, 8, 27)" );
//}
//
// ============================================================================
// TEST 13: String Comparison Predicates
// ============================================================================
//void test_string_comparison_predicates()
//{
//    TEST_START( "String Comparison Predicates" );
//
//    Math_Set<std::string> words1, words2;
//    words1.Insert_Element( "apple" );
//    words1.Insert_Element( "banana" );
//    words1.Insert_Element( "cherry" );
//
//    words2.Insert_Element( "ant" );
//    words2.Insert_Element( "bear" );
//    words2.Insert_Element( "cat" );
//
//    std::cout << "Words1: " << words1 << std::endl;
//    std::cout << "Words2: " << words2 << std::endl;
//
//    TEST_SECTION( "∀x∈words1 ∃y∈words2: x and y start with same letter" );
//    auto same_first_letter = [] ( const std::string & s1, const std::string & s2 )
//    {
//        return !s1.empty() && !s2.empty() && s1 [ 0 ] == s2 [ 0 ];
//    };
//    bool result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Universal_Existential(
//        words1, words2, same_first_letter );
//
//     Let me fix the test
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Universal_Existential(
//        words1, words2, same_first_letter );
//    TEST_ASSERT( result == true, "Each word in set1 has a word in set2 with same first letter" );
//
//    TEST_SECTION( "∃x∈words1 ∀y∈words2: x is lexicographically greater" );
//    auto lex_greater = [] ( const std::string & s1, const std::string & s2 )
//    {
//        return s1 > s2;
//    };
//    result = Nested_Quantifier_Validator<std::string, std::string>::Nested_Quantifier_Existential_Universal(
//        words1, words2, lex_greater );
//    TEST_ASSERT( result == true, "∃x that is lexicographically greater than all y (cherry > all in words2)" );
//
//    TEST_SECTION( "Palindrome Detection" );
//    Math_Set<std::string> test_words;
//    test_words.Insert_Element( "racecar" );
//    test_words.Insert_Element( "hello" );
//    test_words.Insert_Element( "level" );
//
//    std::cout << "Test words: " << test_words << std::endl;
//
//    auto is_palindrome = [] ( const std::string & s )
//    {
//        std::string rev( s.rbegin(), s.rend() );
//        return s == rev;
//    };
//    result = Single_Quantifier_Validator<std::string>::Existential_Quantifier( test_words, is_palindrome );
//    TEST_ASSERT( result == true, "∃ palindrome (racecar, level)" );
//}
//
// ============================================================================
// TEST 14: Mixed Type Nested Quantifiers
// ============================================================================
//void test_mixed_type_quantifiers()
//{
//    TEST_START( "Mixed Type Nested Quantifiers" );
//
//    Math_Set<int> numbers;
//    numbers.Insert_Element( 3 );
//    numbers.Insert_Element( 5 );
//    numbers.Insert_Element( 7 );
//
//    Math_Set<std::string> words;
//    words.Insert_Element( "hi" );
//    words.Insert_Element( "test" );
//    words.Insert_Element( "hello" );
//
//    std::cout << "Numbers: " << numbers << std::endl;
//    std::cout << "Words: " << words << std::endl;
//
//    TEST_SECTION( "∀n∈numbers ∃w∈words: len(w) > n" );
//    auto length_greater = [] ( int n, const std::string & w )
//    {
//        return static_cast< int >( w.length() ) > n;
//    };
//    bool result = Nested_Quantifier_Validator<int, std::string>::Nested_Quantifier_Universal_Existential(
//        numbers, words, length_greater );
//    TEST_ASSERT( result == false, "Not all numbers have a word with length > n (7 fails)" );
//
//    TEST_SECTION( "∃n∈numbers ∀w∈words: len(w) >= n" );
//    auto length_gte = [] ( int n, const std::string & w )
//    {
//        return static_cast< int >( w.length() ) >= n;
//    };
//    result = Nested_Quantifier_Validator<int, std::string>::Nested_Quantifier_Existential_Universal(
//        numbers, words, length_gte );
//    TEST_ASSERT( result == false, "No n where all words have length >= n" );
//
//    TEST_SECTION( "∀w∈words ∃n∈numbers: w contains digit of n" );
//    auto contains_digit = [] ( const std::string & w, int n )
//    {
//        std::string n_str = std::to_string( n );
//        for ( char digit : n_str )
//        {
//            if ( w.find( digit ) != std::string::npos ) return true;
//        }
//        return false;
//    };
//    result = Nested_Quantifier_Validator<std::string, int>::Nested_Quantifier_Universal_Existential(
//        words, numbers, contains_digit );
//    TEST_ASSERT( result == false, "Not all words contain digits from numbers" );
//}
//
// ============================================================================
// TEST 15: Stress Test with Larger Sets
// ============================================================================
//void test_stress_quantifiers()
//{
//    TEST_START( "Stress Test with Larger Sets" );
//
//    TEST_SECTION( "Large Set Universal Quantifier" );
//    Math_Set<int> large_set;
//    for ( int i = 0; i < 100; i++ )
//    {
//        large_set.Insert_Element( i * 2 );  // All even numbers
//    }
//
//    auto is_even = [] ( int x )
//    {
//        return x % 2 == 0;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( large_set, is_even );
//    TEST_ASSERT( result == true, "All 100 elements are even" );
//
//    TEST_SECTION( "Large Set Existential" );
//    auto greater_than_150 = [] ( int x )
//    {
//        return x > 150;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( large_set, greater_than_150 );
//    TEST_ASSERT( result == true, "∃x > 150 in large set" );
//
//    TEST_SECTION( "Nested with Medium Sets" );
//    Math_Set<int> A, B;
//    for ( int i = 1; i <= 20; i++ )
//    {
//        A.Insert_Element( i );
//    }
//    for ( int i = 10; i <= 30; i++ )
//    {
//        B.Insert_Element( i );
//    }
//
//    auto sum_gt_25 = [] ( int x, int y )
//    {
//        return x + y > 25;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Existential( A, B, sum_gt_25 );
//    TEST_ASSERT( result == true, "∃x∈A ∃y∈B: x+y > 25" );
//
//    auto product_even = [] ( int x, int y )
//    {
//        return ( x * y ) % 2 == 0;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal( A, B, product_even );
//    TEST_ASSERT( result == false, "Not all products are even (odd × odd = odd)" );
//}
//
// ============================================================================
// TEST 16: Real-World Scenarios
// ============================================================================
//void test_real_world_scenarios()
//{
//    TEST_START( "Real-World Scenarios" );
//
//    TEST_SECTION( "Student Grades: All Passed" );
//    Math_Set<int> grades;
//    grades.Insert_Element( 75 );
//    grades.Insert_Element( 82 );
//    grades.Insert_Element( 91 );
//    grades.Insert_Element( 68 );
//
//    std::cout << "Grades: " << grades << std::endl;
//
//    auto passed = [] ( int grade )
//    {
//        return grade >= 60;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( grades, passed );
//    TEST_ASSERT( result == true, "All students passed (grade >= 60)" );
//
//    auto honor_roll = [] ( int grade )
//    {
//        return grade >= 90;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( grades, honor_roll );
//    TEST_ASSERT( result == true, "At least one student on honor roll (91)" );
//
//    TEST_SECTION( "Product Prices: Budget Check" );
//    Math_Set<int> prices;
//    prices.Insert_Element( 10 );
//    prices.Insert_Element( 25 );
//    prices.Insert_Element( 50 );
//    prices.Insert_Element( 15 );
//
//    Math_Set<int> budgets;
//    budgets.Insert_Element( 20 );
//    budgets.Insert_Element( 30 );
//    budgets.Insert_Element( 60 );
//
//    std::cout << "Prices: " << prices << std::endl;
//    std::cout << "Budgets: " << budgets << std::endl;
//
//    auto affordable = [] ( int budget, int price )
//    {
//        return budget >= price;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Existential_Universal(
//        budgets, prices, affordable );
//    TEST_ASSERT( result == true, "∃ budget that can afford all items (60 >= all prices)" );
//
//    TEST_SECTION( "File Extensions Validation" );
//    Math_Set<std::string> filenames;
//    filenames.Insert_Element( "document.pdf" );
//    filenames.Insert_Element( "image.jpg" );
//    filenames.Insert_Element( "data.csv" );
//
//    std::cout << "Files: " << filenames << std::endl;
//
//    auto has_extension = [] ( const std::string & filename )
//    {
//        return filename.find( '.' ) != std::string::npos;
//    };
//    result = Single_Quantifier_Validator<std::string>::Universal_Quantifier( filenames, has_extension );
//    TEST_ASSERT( result == true, "All files have extensions" );
//
//    auto is_pdf = [] ( const std::string & filename )
//    {
//        return filename.length() >= 4 &&
//            filename.substr( filename.length() - 4 ) == ".pdf";
//    };
//    result = Single_Quantifier_Validator<std::string>::Unique_Existential_Quantifier( filenames, is_pdf );
//    TEST_ASSERT( result == true, "Exactly one PDF file" );
//}
//
// ============================================================================
// TEST 17: Boundary Conditions
// ============================================================================
//void test_boundary_conditions()
//{
//    TEST_START( "Boundary Conditions" );
//
//    TEST_SECTION( "Singleton Set" );
//    Math_Set<int> singleton;
//    singleton.Insert_Element( 42 );
//
//    auto always_true = [] ( int x )
//    {
//        return true;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( singleton, always_true );
//    TEST_ASSERT( result == true, "∀x in singleton: true" );
//
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( singleton, always_true );
//    TEST_ASSERT( result == true, "∃x in singleton: true" );
//
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( singleton, always_true );
//    TEST_ASSERT( result == true, "∃!x in singleton: true" );
//
//    TEST_SECTION( "Nested with Singletons" );
//    Math_Set<int> single1, single2;
//    single1.Insert_Element( 5 );
//    single2.Insert_Element( 10 );
//
//    auto sum_15 = [] ( int x, int y )
//    {
//        return x + y == 15;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal(
//        single1, single2, sum_15 );
//    TEST_ASSERT( result == true, "Singleton × Singleton with matching predicate" );
//
//    TEST_SECTION( "Very Large Values" );
//    Math_Set<int> large_values;
//    large_values.Insert_Element( 1000000 );
//    large_values.Insert_Element( 2000000 );
//    large_values.Insert_Element( 3000000 );
//
//    auto exceeds_million = [] ( int x )
//    {
//        return x >= 1000000;
//    };
//    result = Single_Quantifier_Validator<int>::Universal_Quantifier( large_values, exceeds_million );
//    TEST_ASSERT( result == true, "All values >= 1 million" );
//}
//
//void test_negative_and_zero()
//{
//    TEST_START( "Negative and Zero Values" );
//
//    Math_Set<int> mixed;
//    mixed.Insert_Element( -5 );
//    mixed.Insert_Element( -2 );
//    mixed.Insert_Element( 0 );
//    mixed.Insert_Element( 3 );
//    mixed.Insert_Element( 7 );
//
//    std::cout << "Mixed set: " << mixed << std::endl;
//
//    TEST_SECTION( "All Non-Positive" );
//    auto non_positive = [] ( int x )
//    {
//        return x <= 0;
//    };
//    bool result = Single_Quantifier_Validator<int>::Universal_Quantifier( mixed, non_positive );
//    TEST_ASSERT( result == false, "Not all are non-positive (3, 7 are positive)" );
//
//    TEST_SECTION( "Exists Zero" );
//    auto is_zero = [] ( int x )
//    {
//        return x == 0;
//    };
//    result = Single_Quantifier_Validator<int>::Existential_Quantifier( mixed, is_zero );
//    TEST_ASSERT( result == true, "Zero exists in set" );
//
//    result = Single_Quantifier_Validator<int>::Unique_Existential_Quantifier( mixed, is_zero );
//    TEST_ASSERT( result == true, "Exactly one zero" );
//
//    TEST_SECTION( "Sign Matching" );
//    Math_Set<int> negatives, positives;
//    negatives.Insert_Element( -3 );
//    negatives.Insert_Element( -1 );
//
//    positives.Insert_Element( 1 );
//    positives.Insert_Element( 3 );
//
//    auto opposite_signs = [] ( int x, int y )
//    {
//        return x * y < 0;
//    };
//    result = Nested_Quantifier_Validator<int, int>::Nested_Quantifier_Universal_Universal(
//        negatives, positives, opposite_signs );
//    TEST_ASSERT( result == true, "All pairs have opposite signs" );
//}
//
// ============================================================================
// MAIN TEST RUNNER
// ============================================================================
//int main()
//{
//    std::cout << BOLD << BLUE;
//    std::cout << "\n";
//    std::cout << "╔════════════════════════════════════════════════╗\n";
//    std::cout << "║                                                ║\n";
//    std::cout << "║   QUANTIFIER VALIDATOR TEST SUITE             ║\n";
//    std::cout << "║   Testing Single & Nested Quantifiers         ║\n";
//    std::cout << "║                                                ║\n";
//    std::cout << "╚════════════════════════════════════════════════╝\n";
//    std::cout << RESET << std::endl;
//
//    try
//    {
//    Single Quantifier Tests
//        test_universal_quantifier_integers();
//        test_universal_quantifier_strings();
//        test_existential_quantifier_integers();
//        test_existential_quantifier_strings();
//        test_unique_existential_quantifier();
//        test_demorgan_laws();
//
//         Nested Quantifier Tests
//        test_nested_universal_universal();
//        test_nested_universal_existential();
//        test_nested_existential_existential();
//        test_nested_existential_universal();
//
//         Advanced Tests
//        test_empty_set_edge_cases();
//        test_complex_mathematical_predicates();
//        test_string_comparison_predicates();
//        test_mixed_type_quantifiers();
//        test_stress_quantifiers();
//        test_real_world_scenarios();
//        test_boundary_conditions();
//        test_negative_and_zero();
//
//         Print summary
//        print_summary();
//
//        return ( failed_tests == 0 ) ? 0 : 1;
//
//    }
//    catch ( const std::exception & e )
//    {
//        std::cout << RED << BOLD << "\n💥 EXCEPTION CAUGHT: " << e.what() << RESET << std::endl;
//        return 1;
//    }
//}
// ============================================================================
// TEST 18: Negative and Zero Values
// ============================================================================
//
// Test for Graph.h
//
//#include "Graph.h"
//
//int main()
//{
//    Graph<int> g( false );  // Undirected graph
//
//    Math_Set<int> nodes;
//    nodes.Insert_Element( 1 );
//    nodes.Insert_Element( 2 );
//    nodes.Insert_Element( 3 );
//    nodes.Insert_Element( 4 );
//
//    g.Initialize_Graph( nodes );
//
//    g.Add_Edge( 1, 2, 1 );
//    g.Add_Edge( 2, 3, 1 );
//    g.Add_Edge( 3, 4, 1 );
//    g.Add_Edge( 4, 1, 1 );
//
//     Print analysis report
//    g.Print_Analysis_Report();
//
//    return 0;
//}
//
//
//** Expected Output : **
//GRAPH ANALYSIS REPORT
//==========================
//Vertices: [4]
//Edges : [4]
//Type : [UNDIRECTED]
//Eulerian Circuit : [YES]
//Eulerian Path : [YES]
//Hamiltonian Cycle : [YES]
//Complete : [NO]
//Connected Components : [1]
//==========================
//
// Test for Graph_Utility.h

//#include "Graph_Utility.h"
//
//// =====================
//// ANSI color codes
//// =====================
//#define RESET   "\033[0m"
//#define RED     "\033[31m"
//#define GREEN   "\033[32m"
//#define YELLOW  "\033[33m"
//#define BLUE    "\033[34m"
//#define CYAN    "\033[36m"
//#define BOLD    "\033[1m"
//
//// =====================
//// Test tracking
//// =====================
//int total_tests = 0;
//int passed_tests = 0;
//int failed_tests = 0;
//
//#define TEST_START(name) \
//    std::cout << CYAN << "\n=== Testing: " << name << " ===" << RESET << std::endl; \
//    total_tests++;
//
//#define TEST_ASSERT(condition, message) \
//    if (condition) { \
//        std::cout << GREEN << "✓ PASS: " << message << RESET << std::endl; \
//        passed_tests++; \
//    } else { \
//        std::cout << RED << "✗ FAIL: " << message << RESET << std::endl; \
//        failed_tests++; \
//    }
//
//#define TEST_SECTION(name) \
//    std::cout << YELLOW << "\n--- " << name << " ---" << RESET << std::endl;
//
//void print_summary()
//{
//    std::cout << BOLD << "\n" << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << BOLD << "TEST SUMMARY" << RESET << std::endl;
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << "Total Tests: " << total_tests << std::endl;
//    std::cout << GREEN << "Passed: " << passed_tests << RESET << std::endl;
//    std::cout << RED << "Failed: " << failed_tests << RESET << std::endl;
//
//    if ( failed_tests == 0 )
//        std::cout << GREEN << BOLD << "\n🎉 ALL TESTS PASSED! 🎉" << RESET << std::endl;
//    else
//        std::cout << RED << BOLD << "\n⚠️  SOME TESTS FAILED ⚠️" << RESET << std::endl;
//
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << "\n" << std::endl;
//}
//
//// =====================
//// Global constants
//// =====================
//constexpr int INF = INT_MAX;
//constexpr int LARGE_GRAPH_SIZE = 50;
//
//// =====================
//// TEST 1: Dijkstra Integers
//// =====================
//void test_dijkstra_integers()
//{
//    TEST_START( "Dijkstra's Shortest Path - Integers" );
//
//    Graph<int> graph( false );
//
//    constexpr int NODE_START = 0;
//    constexpr int NODE_END = 5;
//    Math_Set<int> nodes;
//    for ( int i = NODE_START; i <= NODE_END; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W0_1 = 4, W0_2 = 1, W1_3 = 1, W2_3 = 5, W0_4 = 3, W4_5 = 2, W3_5 = 3;
//
//    graph.Add_Edge( 0, 1, W0_1 );
//    graph.Add_Edge( 0, 2, W0_2 );
//    graph.Add_Edge( 1, 3, W1_3 );
//    graph.Add_Edge( 2, 3, W2_3 );
//    graph.Add_Edge( 0, 4, W0_4 );
//    graph.Add_Edge( 4, 5, W4_5 );
//    graph.Add_Edge( 3, 5, W3_5 );
//
//    int start = NODE_START, end = 3;
//    TEST_SECTION( "Path from 0 to 3" );
//    auto result1 = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result1 << std::endl;
//    TEST_ASSERT( result1.m_Path_Exists, "Path exists from 0 to 3" );
//    TEST_ASSERT( result1.m_Total_Cost == 5, "Shortest path cost is 5 (0->1->3)" );
//    TEST_ASSERT( result1.m_Path.size() == 3, "Path has 3 nodes" );
//
//    end = 5;
//    TEST_SECTION( "Path from 0 to 5" );
//    auto result2 = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result2 << std::endl;
//    TEST_ASSERT( result2.m_Path_Exists, "Path exists from 0 to 5" );
//    TEST_ASSERT( result2.m_Total_Cost == 5, "Shortest path cost is 5 (0->4->5)" );
//
//    end = start;
//    TEST_SECTION( "Path to same node" );
//    auto result3 = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result3 << std::endl;
//    TEST_ASSERT( result3.m_Path_Exists, "Path to self exists" );
//    TEST_ASSERT( result3.m_Total_Cost == 0, "Cost is 0" );
//}
//
//// =====================
//// TEST 2: Dijkstra Characters
//// =====================
//void test_dijkstra_characters()
//{
//    TEST_START( "Dijkstra's Shortest Path - Characters" );
//
//    Graph<char> graph( false );
//
//    Math_Set<char> nodes;
//    constexpr char A = 'A', B = 'B', C = 'C', D = 'D', E = 'E';
//    nodes.Insert_Element( A );
//    nodes.Insert_Element( B );
//    nodes.Insert_Element( C );
//    nodes.Insert_Element( D );
//    nodes.Insert_Element( E );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W_AB = 2, W_BC = 3, W_AD = 4, W_DE = 1, W_CE = 2;
//    graph.Add_Edge( A, B, W_AB );
//    graph.Add_Edge( B, C, W_BC );
//    graph.Add_Edge( A, D, W_AD );
//    graph.Add_Edge( D, E, W_DE );
//    graph.Add_Edge( C, E, W_CE );
//
//    TEST_SECTION( "Path from A to E" );
//    char start = A, end = E;
//    auto result = Dijikstras_Shortest_Path<char>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result << std::endl;
//    TEST_ASSERT( result.m_Path_Exists, "Path exists from A to E" );
//    TEST_ASSERT( result.m_Total_Cost == 5, "Shortest path cost is 5 (A->D->E)" );
//
//    TEST_SECTION( "Path from A to C" );
//    end = C;
//    auto result2 = Dijikstras_Shortest_Path<char>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result2 << std::endl;
//    TEST_ASSERT( result2.m_Path_Exists, "Path exists from A to C" );
//    TEST_ASSERT( result2.m_Total_Cost == 5, "Shortest path cost is 5 (A->B->C)" );
//}
//
//// =====================
//// TEST 3: Dijkstra No Path
//// =====================
//void test_dijkstra_no_path()
//{
//    TEST_START( "Dijkstra's - Disconnected Graph" );
//
//    Graph<int> graph( false );
//
//    constexpr int NODE_MIN = 1, NODE_MAX = 6;
//    Math_Set<int> nodes;
//    for ( int i = NODE_MIN; i <= NODE_MAX; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W = 1;
//    graph.Add_Edge( 1, 2, W );
//    graph.Add_Edge( 2, 3, W );
//    graph.Add_Edge( 4, 5, W );
//    graph.Add_Edge( 5, 6, W );
//
//    TEST_SECTION( "No path between components" );
//    int start = 1, end = 6;
//    auto result = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( graph, start, end );
//    std::cout << result << std::endl;
//    TEST_ASSERT( !result.m_Path_Exists, "No path exists between disconnected components" );
//    TEST_ASSERT( result.m_Total_Cost == INF, "Cost is INF" );
//}
//
//// =====================
//// TEST 4: Prim MST Basic Integers
//// =====================
//void test_prims_mst_basic()
//{
//    TEST_START( "Prim's MST - Basic Integer Graph" );
//
//    Graph<int> graph( false );
//
//    constexpr int NODE_START = 0, NODE_END = 4;
//    Math_Set<int> nodes;
//    for ( int i = NODE_START; i <= NODE_END; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W0_1 = 2, W0_3 = 6, W1_2 = 3, W1_3 = 8, W1_4 = 5, W2_4 = 7, W3_4 = 9;
//    graph.Add_Edge( 0, 1, W0_1 );
//    graph.Add_Edge( 0, 3, W0_3 );
//    graph.Add_Edge( 1, 2, W1_2 );
//    graph.Add_Edge( 1, 3, W1_3 );
//    graph.Add_Edge( 1, 4, W1_4 );
//    graph.Add_Edge( 2, 4, W2_4 );
//    graph.Add_Edge( 3, 4, W3_4 );
//
//    Math_Set<int> no_constraints;
//    int start = NODE_START;
//    TEST_SECTION( "MST without constraints" );
//    auto result = Constrained_Prims_MST<int>::Constrained_Prims_MST_Calculation( graph, no_constraints, start );
//    std::cout << result << std::endl;
//    TEST_ASSERT( result.m_MST_Exists, "MST exists" );
//    TEST_ASSERT( result.m_Edges.size() == NODE_END, "MST has V-1 = 4 edges" );
//    TEST_ASSERT( result.m_Total_Weight == 16, "MST total weight is 16" );
//}
//
//// =====================
//// TEST 5: Prim MST With Constraints
//// =====================
//void test_prims_mst_constrained()
//{
//    TEST_START( "Prim's MST - With Constraints" );
//
//    Graph<int> graph( false );
//
//    constexpr int NODE_START = 0, NODE_END = 3;
//    Math_Set<int> nodes;
//    for ( int i = NODE_START; i <= NODE_END; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W0_1 = 4, W1_3 = 5, W0_2 = 1, W2_3 = 2;
//    graph.Add_Edge( 0, 1, W0_1 );
//    graph.Add_Edge( 1, 3, W1_3 );
//    graph.Add_Edge( 0, 2, W0_2 );
//    graph.Add_Edge( 2, 3, W2_3 );
//
//    Math_Set<int> must_include;
//    must_include.Insert_Element( 1 );
//    must_include.Insert_Element( 3 );
//
//    int start = NODE_START;
//    TEST_SECTION( "Must include nodes {1,3}" );
//    auto result = Constrained_Prims_MST<int>::Constrained_Prims_MST_Calculation( graph, must_include, start );
//    std::cout << result << std::endl;
//
//    TEST_ASSERT( result.m_MST_Exists, "Constrained MST exists" );
//    TEST_ASSERT( result.m_Edges.size() == NODE_END, "MST has 3 edges" );
//
//    // Check nodes included
//    bool has_node_1 = false, has_node_3 = false;
//    for ( const auto & edge : result.m_Edges )
//    {
//        if ( edge.first.first == 1 || edge.first.second == 1 ) has_node_1 = true;
//        if ( edge.first.first == 3 || edge.first.second == 3 ) has_node_3 = true;
//    }
//    TEST_ASSERT( has_node_1, "Node 1 is in MST" );
//    TEST_ASSERT( has_node_3, "Node 3 is in MST" );
//}
//
//// =====================
//// TEST 6: Prim MST Characters
//// =====================
//void test_prims_mst_characters()
//{
//    TEST_START( "Prim's MST - Character Graph" );
//
//    Graph<char> graph( false );
//    constexpr char A = 'A', B = 'B', C = 'C', D = 'D';
//    Math_Set<char> nodes;
//    nodes.Insert_Element( A );
//    nodes.Insert_Element( B );
//    nodes.Insert_Element( C );
//    nodes.Insert_Element( D );
//    graph.Initialize_Graph( nodes );
//
//    constexpr int W_AB = 1, W_AC = 3, W_BC = 1, W_BD = 6, W_CD = 2;
//    graph.Add_Edge( A, B, W_AB );
//    graph.Add_Edge( A, C, W_AC );
//    graph.Add_Edge( B, C, W_BC );
//    graph.Add_Edge( B, D, W_BD );
//    graph.Add_Edge( C, D, W_CD );
//
//    Math_Set<char> no_constraints;
//    char start = A;
//    TEST_SECTION( "MST starting from A" );
//    auto result = Constrained_Prims_MST<char>::Constrained_Prims_MST_Calculation( graph, no_constraints, start );
//    std::cout << result << std::endl;
//    TEST_ASSERT( result.m_MST_Exists, "MST exists" );
//    TEST_ASSERT( result.m_Edges.size() == 3, "MST has 3 edges" );
//    TEST_ASSERT( result.m_Total_Weight == 4, "MST weight is 4 (A-B:1, B-C:1, C-D:2)" );
//}
//
//// =====================
//// TEST 7: Critical Nodes Integration
//// =====================
//void test_critical_nodes_integration()
//{
//    TEST_START( "Integration: Critical Nodes + MST" );
//
//    Graph<int> graph( false );
//    constexpr int NODE_MIN = 1, NODE_MAX = 8;
//    Math_Set<int> nodes;
//    for ( int i = NODE_MIN; i <= NODE_MAX; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Edge weights
//    constexpr int W1_2 = 1, W1_3 = 3, W2_3 = 1, W2_4 = 4, W3_5 = 2, W4_6 = 2,
//        W5_6 = 1, W5_7 = 3, W6_8 = 2, W7_8 = 1;
//    graph.Add_Edge( 1, 2, W1_2 );
//    graph.Add_Edge( 1, 3, W1_3 );
//    graph.Add_Edge( 2, 3, W2_3 );
//    graph.Add_Edge( 2, 4, W2_4 );
//    graph.Add_Edge( 3, 5, W3_5 );
//    graph.Add_Edge( 4, 6, W4_6 );
//    graph.Add_Edge( 5, 6, W5_6 );
//    graph.Add_Edge( 5, 7, W5_7 );
//    graph.Add_Edge( 6, 8, W6_8 );
//    graph.Add_Edge( 7, 8, W7_8 );
//
//    Math_Set<int> high_value;
//    high_value.Insert_Element( 1 );
//    high_value.Insert_Element( 2 );
//    high_value.Insert_Element( 3 );
//
//    Math_Set<int> frequently_accessed;
//    frequently_accessed.Insert_Element( 2 );
//    frequently_accessed.Insert_Element( 3 );
//    frequently_accessed.Insert_Element( 5 );
//
//    Math_Set<int> gateway;
//    gateway.Insert_Element( 6 );
//
//    TEST_SECTION( "Generate Critical Nodes" );
//    auto critical_nodes = Critical_Node_Generator<int>::Generate_Priority_Nodes(
//        high_value, frequently_accessed, gateway );
//    std::cout << "Critical Nodes: " << critical_nodes << std::endl;
//    TEST_ASSERT( critical_nodes.Set_Cardinality() == 3, "3 critical nodes (2,3,6)" );
//
//    TEST_SECTION( "MST with Critical Node Constraints" );
//    int start = NODE_MIN;
//    auto mst = Constrained_Prims_MST<int>::Constrained_Prims_MST_Calculation( graph, critical_nodes, start );
//    std::cout << mst << std::endl;
//    TEST_ASSERT( mst.m_MST_Exists, "Constrained MST exists" );
//}
//
//// =====================
//// TEST 8: Edge Cases
//// =====================
//void test_edge_cases()
//{
//    TEST_START( "Edge Cases" );
//
//    TEST_SECTION( "Single Node Graph" );
//    Graph<int> single_node( false );
//    Math_Set<int> one_node;
//    int SINGLE_NODE = 42;
//    one_node.Insert_Element( SINGLE_NODE );
//    single_node.Initialize_Graph( one_node );
//    auto result1 = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( single_node, SINGLE_NODE, SINGLE_NODE );
//    TEST_ASSERT( result1.m_Path_Exists, "Path to self exists" );
//    TEST_ASSERT( result1.m_Total_Cost == 0, "Cost is 0" );
//
//    TEST_SECTION( "Empty Graph" );
//    Graph<int> empty_graph( false );
//    Math_Set<int> empty_nodes;
//    int n1 = 1, n2 = 2;
//    empty_graph.Initialize_Graph( empty_nodes );
//    auto result2 = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( empty_graph, n1, n2);
//    TEST_ASSERT( !result2.m_Path_Exists, "No path in empty graph" );
//
//    TEST_SECTION( "MST on Disconnected Graph" );
//    Graph<int> disconnected( false );
//    Math_Set<int> disc_nodes;
//    disc_nodes.Insert_Element( 1 );
//    disc_nodes.Insert_Element( 2 );
//    disc_nodes.Insert_Element( 3 );
//    disc_nodes.Insert_Element( 4 );
//    disconnected.Initialize_Graph( disc_nodes );
//    disconnected.Add_Edge( 1, 2, 1 );
//    disconnected.Add_Edge( 3, 4, 1 );
//    Math_Set<int> no_constraints;
//    auto mst = Constrained_Prims_MST<int>::Constrained_Prims_MST_Calculation( disconnected, no_constraints, 1 );
//    TEST_ASSERT( !mst.m_MST_Exists, "MST fails on disconnected graph" );
//}
//
//// =====================
//// TEST 9: Large Graph Performance
//// =====================
//void test_large_graph()
//{
//    TEST_START( "Large Graph Performance" );
//
//    Graph<int> large_graph( false );
//    Math_Set<int> nodes;
//    constexpr int NODE_COUNT = LARGE_GRAPH_SIZE;
//    for ( int i = 0; i < NODE_COUNT; i++ )
//        nodes.Insert_Element( i );
//    large_graph.Initialize_Graph( nodes );
//
//    // Add edges in a simple weighted chain
//    constexpr int EDGE_WEIGHT = 1;
//    for ( int i = 0; i < NODE_COUNT - 1; i++ )
//        large_graph.Add_Edge( i, i + 1, EDGE_WEIGHT );
//
//    int start = 0;
//    int end = NODE_COUNT - 1;
//    TEST_SECTION( "Dijkstra's on large linear graph" );
//    auto result = Dijikstras_Shortest_Path<int>::Dijikstras_Shortest_Path_Calculation( large_graph, start, end );
//    std::cout << result << std::endl;
//    TEST_ASSERT( result.m_Path_Exists, "Path exists from start to end" );
//    TEST_ASSERT( result.m_Total_Cost == NODE_COUNT - 1, "Total cost equals NODE_COUNT - 1" );
//    TEST_ASSERT( result.m_Path.size() == NODE_COUNT, "Path includes all nodes" );
//}
//
//// =====================
//// TEST 10: Fully Connected Graph
//// =====================
//void test_fully_connected_graph()
//{
//    TEST_START( "Fully Connected Graph MST" );
//
//    Graph<int> graph( false );
//    constexpr int FC_NODE_COUNT = 5;
//    Math_Set<int> nodes;
//    for ( int i = 0; i < FC_NODE_COUNT; i++ )
//        nodes.Insert_Element( i );
//    graph.Initialize_Graph( nodes );
//
//    // Add all possible edges with weight = i + j + 1 (arbitrary deterministic weight)
//    for ( int i = 0; i < FC_NODE_COUNT; i++ )
//        for ( int j = i + 1; j < FC_NODE_COUNT; j++ )
//            graph.Add_Edge( i, j, i + j + 1 );
//
//    Math_Set<int> no_constraints;
//    int start = 0;
//    TEST_SECTION( "MST of fully connected graph" );
//    auto result = Constrained_Prims_MST<int>::Constrained_Prims_MST_Calculation( graph, no_constraints, start );
//    std::cout << result << std::endl;
//    TEST_ASSERT( result.m_MST_Exists, "MST exists" );
//    TEST_ASSERT( result.m_Edges.size() == FC_NODE_COUNT - 1, "MST has V-1 edges" );
//
//    // Calculate expected total weight manually: MST picks minimal connecting edges
//    // Correct calculation:
//    // (0,1)=2, (0,2)=3, (0,3)=4, (0,4)=5
//    int expected_weight = 2 + 3 + 4 + 5;  // = 14
//    TEST_ASSERT( result.m_Total_Weight == expected_weight, "MST total weight is correct" );
//}
//
//// =====================
//// RUN ALL TESTS
//// =====================
//int main()
//{
//    test_dijkstra_integers();
//    test_dijkstra_characters();
//    test_dijkstra_no_path();
//    test_prims_mst_basic();
//    test_prims_mst_constrained();
//    test_prims_mst_characters();
//    test_critical_nodes_integration();
//    test_edge_cases();
//    test_large_graph();
//    test_fully_connected_graph();
//
//    print_summary();
//    return 0;
//}

// Tests For Functions and Relations
//#include "Functions.h"
//
//// ANSI color codes
//#define RESET   "\033[0m"
//#define RED     "\033[31m"
//#define GREEN   "\033[32m"
//#define YELLOW  "\033[33m"
//#define BLUE    "\033[34m"
//#define CYAN    "\033[36m"
//#define BOLD    "\033[1m"
//
//int total_tests = 0;
//int passed_tests = 0;
//int failed_tests = 0;
//
//#define TEST_START(name) \
//    std::cout << CYAN << "\n=== Testing: " << name << " ===" << RESET << std::endl; \
//    total_tests++;
//
//#define TEST_ASSERT(condition, message) \
//    if (condition) { \
//        std::cout << GREEN << "✓ PASS: " << message << RESET << std::endl; \
//        passed_tests++; \
//    } else { \
//        std::cout << RED << "✗ FAIL: " << message << RESET << std::endl; \
//        failed_tests++; \
//    }
//
//#define TEST_SECTION(name) \
//    std::cout << YELLOW << "\n--- " << name << " ---" << RESET << std::endl;
//
//void print_summary()
//{
//    std::cout << BOLD << "\n" << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << BOLD << "TEST SUMMARY" << RESET << std::endl;
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << std::endl;
//    std::cout << "Total Tests: " << total_tests << std::endl;
//    std::cout << GREEN << "Passed: " << passed_tests << RESET << std::endl;
//    std::cout << RED << "Failed: " << failed_tests << RESET << std::endl;
//
//    if ( failed_tests == 0 )
//    {
//        std::cout << GREEN << BOLD << "\n🎉 ALL TESTS PASSED! 🎉" << RESET << std::endl;
//    }
//    else
//    {
//        std::cout << RED << BOLD << "\n⚠️  SOME TESTS FAILED ⚠️" << RESET << std::endl;
//    }
//    std::cout << BOLD << std::string( 50, '=' ) << RESET << "\n" << std::endl;
//}
//
//// ============================================================================
//// TEST 1: Relation - Reflexivity
//// ============================================================================
//void test_relation_reflexive()
//{
//    TEST_START( "Relation - Reflexivity" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Reflexive Relation" );
//    Relation<int> reflexive_rel( domain );
//    reflexive_rel.Add_Pair( 1, 1 );
//    reflexive_rel.Add_Pair( 2, 2 );
//    reflexive_rel.Add_Pair( 3, 3 );
//
//    TEST_ASSERT( reflexive_rel.Is_Reflexive(), "All elements have self-loops" );
//
//    TEST_SECTION( "Non-Reflexive Relation" );
//    Relation<int> non_reflexive( domain );
//    non_reflexive.Add_Pair( 1, 1 );
//    non_reflexive.Add_Pair( 2, 2 );
//    // Missing (3,3)
//
//    TEST_ASSERT( !non_reflexive.Is_Reflexive(), "Missing self-loop detected" );
//
//    TEST_SECTION( "Empty Relation" );
//    Relation<int> empty_rel( domain );
//
//    TEST_ASSERT( !empty_rel.Is_Reflexive(), "Empty relation is not reflexive" );
//}
//
//// ============================================================================
//// TEST 2: Relation - Symmetry
//// ============================================================================
//void test_relation_symmetric()
//{
//    TEST_START( "Relation - Symmetry" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Symmetric Relation" );
//    Relation<int> symmetric_rel( domain );
//    symmetric_rel.Add_Pair( 1, 2 );
//    symmetric_rel.Add_Pair( 2, 1 );
//    symmetric_rel.Add_Pair( 2, 3 );
//    symmetric_rel.Add_Pair( 3, 2 );
//
//    TEST_ASSERT( symmetric_rel.Is_Symmetric(), "All pairs are symmetric" );
//
//    TEST_SECTION( "Non-Symmetric Relation" );
//    Relation<int> non_symmetric( domain );
//    non_symmetric.Add_Pair( 1, 2 );
//    // Missing (2,1)
//
//    TEST_ASSERT( !non_symmetric.Is_Symmetric(), "Missing symmetric pair detected" );
//
//    TEST_SECTION( "Self-loops (Always Symmetric)" );
//    Relation<int> self_loops( domain );
//    self_loops.Add_Pair( 1, 1 );
//    self_loops.Add_Pair( 2, 2 );
//
//    TEST_ASSERT( self_loops.Is_Symmetric(), "Self-loops are symmetric" );
//}
//
//// ============================================================================
//// TEST 3: Relation - Antisymmetry
//// ============================================================================
//void test_relation_antisymmetric()
//{
//    TEST_START( "Relation - Antisymmetry" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Antisymmetric Relation" );
//    Relation<int> antisym( domain );
//    antisym.Add_Pair( 1, 2 );
//    antisym.Add_Pair( 2, 3 );
//    antisym.Add_Pair( 1, 1 );  // Self-loops allowed
//
//    TEST_ASSERT( antisym.Is_AntiSymmetric(), "No symmetric pairs except self-loops" );
//
//    TEST_SECTION( "Non-Antisymmetric Relation" );
//    Relation<int> non_antisym( domain );
//    non_antisym.Add_Pair( 1, 2 );
//    non_antisym.Add_Pair( 2, 1 );  // Symmetric pair where 1 ≠ 2
//
//    TEST_ASSERT( !non_antisym.Is_AntiSymmetric(), "Symmetric pair detected" );
//
//    TEST_SECTION( "Self-loops Only (Antisymmetric)" );
//    Relation<int> only_loops( domain );
//    only_loops.Add_Pair( 1, 1 );
//    only_loops.Add_Pair( 2, 2 );
//
//    TEST_ASSERT( only_loops.Is_AntiSymmetric(), "Self-loops are antisymmetric" );
//}
//
//// ============================================================================
//// TEST 4: Relation - Transitivity
//// ============================================================================
//void test_relation_transitive()
//{
//    TEST_START( "Relation - Transitivity" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//    domain.Insert_Element( 4 );
//
//    TEST_SECTION( "Transitive Relation" );
//    Relation<int> transitive_rel( domain );
//    transitive_rel.Add_Pair( 1, 2 );
//    transitive_rel.Add_Pair( 2, 3 );
//    transitive_rel.Add_Pair( 1, 3 );  // Transitive closure
//    transitive_rel.Add_Pair( 3, 4 );
//    transitive_rel.Add_Pair( 1, 4 );  // Transitive closure
//    transitive_rel.Add_Pair( 2, 4 );  // Transitive closure
//
//    TEST_ASSERT( transitive_rel.Is_Transitive(), "All transitive pairs present" );
//
//    TEST_SECTION( "Non-Transitive Relation" );
//    Relation<int> non_transitive( domain );
//    non_transitive.Add_Pair( 1, 2 );
//    non_transitive.Add_Pair( 2, 3 );
//    // Missing (1,3)
//
//    TEST_ASSERT( !non_transitive.Is_Transitive(), "Missing transitive pair detected" );
//}
//
//// ============================================================================
//// TEST 5: Relation - Equivalence Relations
//// ============================================================================
//void test_equivalence_relations()
//{
//    TEST_START( "Equivalence Relations" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Valid Equivalence Relation" );
//    Relation<int> equiv( domain );
//    // Reflexive
//    equiv.Add_Pair( 1, 1 );
//    equiv.Add_Pair( 2, 2 );
//    equiv.Add_Pair( 3, 3 );
//    // Symmetric pairs
//    equiv.Add_Pair( 1, 2 );
//    equiv.Add_Pair( 2, 1 );
//    // Transitive
//    // All necessary pairs present
//
//    TEST_ASSERT( equiv.Is_Reflexive(), "Equivalence is reflexive" );
//    TEST_ASSERT( equiv.Is_Symmetric(), "Equivalence is symmetric" );
//    TEST_ASSERT( equiv.Is_Transitive(), "Equivalence is transitive" );
//    TEST_ASSERT( equiv.Is_Equivalence_Relation(), "Valid equivalence relation" );
//
//    TEST_SECTION( "Invalid Equivalence (Missing Reflexivity)" );
//    Relation<int> not_equiv( domain );
//    not_equiv.Add_Pair( 1, 2 );
//    not_equiv.Add_Pair( 2, 1 );
//
//    TEST_ASSERT( !not_equiv.Is_Equivalence_Relation(), "Not reflexive, not equivalence" );
//}
//
//// ============================================================================
//// TEST 6: Relation - Partial Orders
//// ============================================================================
//void test_partial_orders()
//{
//    TEST_START( "Partial Order Relations" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Valid Partial Order (≤)" );
//    Relation<int> partial_order( domain );
//    // Reflexive
//    partial_order.Add_Pair( 1, 1 );
//    partial_order.Add_Pair( 2, 2 );
//    partial_order.Add_Pair( 3, 3 );
//    // Antisymmetric and Transitive
//    partial_order.Add_Pair( 1, 2 );
//    partial_order.Add_Pair( 2, 3 );
//    partial_order.Add_Pair( 1, 3 );  // Transitive
//
//    TEST_ASSERT( partial_order.Is_Reflexive(), "Partial order is reflexive" );
//    TEST_ASSERT( partial_order.Is_AntiSymmetric(), "Partial order is antisymmetric" );
//    TEST_ASSERT( partial_order.Is_Transitive(), "Partial order is transitive" );
//    TEST_ASSERT( partial_order.Is_Partial_Order(), "Valid partial order" );
//
//    TEST_SECTION( "Invalid Partial Order (Symmetric)" );
//    Relation<int> not_partial( domain );
//    not_partial.Add_Pair( 1, 1 );
//    not_partial.Add_Pair( 2, 2 );
//    not_partial.Add_Pair( 1, 2 );
//    not_partial.Add_Pair( 2, 1 );  // Makes it symmetric, not antisymmetric
//
//    TEST_ASSERT( !not_partial.Is_Partial_Order(), "Symmetric, not partial order" );
//}
//
//// ============================================================================
//// TEST 7: Relation - Closures
//// ============================================================================
//void test_relation_closures()
//{
//    TEST_START( "Relation Closures" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Reflexive Closure" );
//    Relation<int> rel( domain );
//    rel.Add_Pair( 1, 2 );
//
//    auto reflexive_closure = rel.Reflexive_Closure();
//
//    TEST_ASSERT( reflexive_closure.Is_Reflexive(), "Reflexive closure is reflexive" );
//    TEST_ASSERT( reflexive_closure.Contains_Pair( 1, 1 ), "Added (1,1)" );
//    TEST_ASSERT( reflexive_closure.Contains_Pair( 2, 2 ), "Added (2,2)" );
//    TEST_ASSERT( reflexive_closure.Contains_Pair( 3, 3 ), "Added (3,3)" );
//    TEST_ASSERT( reflexive_closure.Contains_Pair( 1, 2 ), "Original pair preserved" );
//
//    TEST_SECTION( "Symmetric Closure" );
//    Relation<int> rel2( domain );
//    rel2.Add_Pair( 1, 2 );
//    rel2.Add_Pair( 2, 3 );
//
//    auto symmetric_closure = rel2.Symmetric_Closure();
//
//    TEST_ASSERT( symmetric_closure.Is_Symmetric(), "Symmetric closure is symmetric" );
//    TEST_ASSERT( symmetric_closure.Contains_Pair( 2, 1 ), "Added reverse (2,1)" );
//    TEST_ASSERT( symmetric_closure.Contains_Pair( 3, 2 ), "Added reverse (3,2)" );
//
//    TEST_SECTION( "Transitive Closure" );
//    Relation<int> rel3( domain );
//    rel3.Add_Pair( 1, 2 );
//    rel3.Add_Pair( 2, 3 );
//
//    auto transitive_closure = rel3.Transitive_Closure();
//
//    TEST_ASSERT( transitive_closure.Is_Transitive(), "Transitive closure is transitive" );
//    TEST_ASSERT( transitive_closure.Contains_Pair( 1, 3 ), "Added transitive pair (1,3)" );
//}
//
//// ============================================================================
//// TEST 8: Relation - String Domain
//// ============================================================================
//void test_relation_strings()
//{
//    TEST_START( "Relation - String Domain" );
//
//    Math_Set<std::string> domain;
//    domain.Insert_Element( "Alice" );
//    domain.Insert_Element( "Bob" );
//    domain.Insert_Element( "Charlie" );
//
//    TEST_SECTION( "Friendship Relation (Symmetric)" );
//    Relation<std::string> friendship( domain );
//    friendship.Add_Pair( "Alice", "Bob" );
//    friendship.Add_Pair( "Bob", "Alice" );
//    friendship.Add_Pair( "Bob", "Charlie" );
//    friendship.Add_Pair( "Charlie", "Bob" );
//
//    TEST_ASSERT( friendship.Is_Symmetric(), "Friendship is symmetric" );
//    TEST_ASSERT( !friendship.Is_Reflexive(), "People not friends with themselves" );
//
//    TEST_SECTION( "Hierarchy Relation (Antisymmetric)" );
//    Relation<std::string> hierarchy( domain );
//    hierarchy.Add_Pair( "Alice", "Alice" );
//    hierarchy.Add_Pair( "Bob", "Bob" );
//    hierarchy.Add_Pair( "Charlie", "Charlie" );
//    hierarchy.Add_Pair( "Alice", "Bob" );
//    hierarchy.Add_Pair( "Bob", "Charlie" );
//    hierarchy.Add_Pair( "Alice", "Charlie" );
//
//    TEST_ASSERT( hierarchy.Is_AntiSymmetric(), "Hierarchy is antisymmetric" );
//    TEST_ASSERT( hierarchy.Is_Transitive(), "Hierarchy is transitive" );
//    TEST_ASSERT( hierarchy.Is_Partial_Order(), "Valid hierarchy (partial order)" );
//}
//
//// ============================================================================
//// TEST 9: Function - Basic Operations
//// ============================================================================
//void test_function_basic()
//{
//    TEST_START( "Function - Basic Operations" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//
//    TEST_SECTION( "Define and Evaluate Function" );
//    Function<int, int> f( domain, codomain );
//    f.Define_Mapping( 1, 10 );
//    f.Define_Mapping( 2, 20 );
//    f.Define_Mapping( 3, 30 );
//
//    auto result1 = f.Evaluate_Function( 1 );
//    TEST_ASSERT( result1.has_value() && result1.value() == 10, "f(1) = 10" );
//
//    auto result2 = f.Evaluate_Function( 2 );
//    TEST_ASSERT( result2.has_value() && result2.value() == 20, "f(2) = 20" );
//
//    auto result3 = f.Evaluate_Function( 3 );
//    TEST_ASSERT( result3.has_value() && result3.value() == 30, "f(3) = 30" );
//
//    TEST_SECTION( "Undefined Input" );
//    auto undefined = f.Evaluate_Function( 99 );
//    TEST_ASSERT( !undefined.has_value(), "f(99) is undefined" );
//
//    TEST_SECTION( "Is_Defined_At" );
//    TEST_ASSERT( f.Is_Defined_At( 1 ), "Function defined at 1" );
//    TEST_ASSERT( f.Is_Defined_At( 2 ), "Function defined at 2" );
//    TEST_ASSERT( !f.Is_Defined_At( 99 ), "Function not defined at 99" );
//}
//
//// ============================================================================
//// TEST 10: Function - Injectivity
//// ============================================================================
//void test_function_injective()
//{
//    TEST_START( "Function - Injectivity (One-to-One)" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//    codomain.Insert_Element( 40 );
//
//    TEST_SECTION( "Injective Function" );
//    Function<int, int> injective( domain, codomain );
//    injective.Define_Mapping( 1, 10 );
//    injective.Define_Mapping( 2, 20 );
//    injective.Define_Mapping( 3, 30 );
//    // Each output has at most one input
//
//    TEST_ASSERT( injective.Is_Injective(), "Each output from unique input" );
//
//    TEST_SECTION( "Non-Injective Function" );
//    Function<int, int> non_injective( domain, codomain );
//    non_injective.Define_Mapping( 1, 10 );
//    non_injective.Define_Mapping( 2, 10 );  // Two inputs map to same output
//    non_injective.Define_Mapping( 3, 30 );
//
//    TEST_ASSERT( !non_injective.Is_Injective(), "Multiple inputs map to 10" );
//}
//
//// ============================================================================
//// TEST 11: Function - Surjectivity
//// ============================================================================
//void test_function_surjective()
//{
//    TEST_START( "Function - Surjectivity (Onto)" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//
//    TEST_SECTION( "Surjective Function" );
//    Function<int, int> surjective( domain, codomain );
//    surjective.Define_Mapping( 1, 10 );
//    surjective.Define_Mapping( 2, 20 );
//    surjective.Define_Mapping( 3, 30 );
//    // Every codomain element is hit
//
//    TEST_ASSERT( surjective.Is_Surjective(), "All codomain elements covered" );
//
//    TEST_SECTION( "Non-Surjective Function" );
//    Math_Set<int> larger_codomain;
//    larger_codomain.Insert_Element( 10 );
//    larger_codomain.Insert_Element( 20 );
//    larger_codomain.Insert_Element( 30 );
//    larger_codomain.Insert_Element( 40 );
//
//    Function<int, int> non_surjective( domain, larger_codomain );
//    non_surjective.Define_Mapping( 1, 10 );
//    non_surjective.Define_Mapping( 2, 20 );
//    non_surjective.Define_Mapping( 3, 30 );
//    // 40 is not hit
//
//    TEST_ASSERT( !non_surjective.Is_Surjective(), "40 not in range" );
//}
//
//// ============================================================================
//// TEST 12: Function - Bijectivity
//// ============================================================================
//void test_function_bijective()
//{
//    TEST_START( "Function - Bijectivity (One-to-One and Onto)" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//
//    TEST_SECTION( "Bijective Function" );
//    Function<int, int> bijective( domain, codomain );
//    bijective.Define_Mapping( 1, 10 );
//    bijective.Define_Mapping( 2, 20 );
//    bijective.Define_Mapping( 3, 30 );
//
//    TEST_ASSERT( bijective.Is_Injective(), "Bijective is injective" );
//    TEST_ASSERT( bijective.Is_Surjective(), "Bijective is surjective" );
//    TEST_ASSERT( bijective.Is_Bijective(), "Function is bijective" );
//
//    TEST_SECTION( "Non-Bijective (Not Injective)" );
//    Function<int, int> not_bijective1( domain, codomain );
//    not_bijective1.Define_Mapping( 1, 10 );
//    not_bijective1.Define_Mapping( 2, 10 );
//    not_bijective1.Define_Mapping( 3, 20 );
//
//    TEST_ASSERT( !not_bijective1.Is_Bijective(), "Not injective" );
//
//    TEST_SECTION( "Non-Bijective (Not Surjective)" );
//    Math_Set<int> larger_codomain;
//    larger_codomain.Insert_Element( 10 );
//    larger_codomain.Insert_Element( 20 );
//    larger_codomain.Insert_Element( 30 );
//    larger_codomain.Insert_Element( 40 );
//
//    Function<int, int> not_bijective2( domain, larger_codomain );
//    not_bijective2.Define_Mapping( 1, 10 );
//    not_bijective2.Define_Mapping( 2, 20 );
//    not_bijective2.Define_Mapping( 3, 30 );
//
//    TEST_ASSERT( !not_bijective2.Is_Bijective(), "Not surjective" );
//}
//
//// ============================================================================
//// TEST 13: Function - Composition
//// ============================================================================
//void test_function_composition()
//{
//    TEST_START( "Function - Composition" );
//
//    Math_Set<int> A, B, C;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//
//    B.Insert_Element( 10 );
//    B.Insert_Element( 20 );
//
//    C.Insert_Element( 100 );
//    C.Insert_Element( 200 );
//
//    TEST_SECTION( "Compose f: A→B with g: B→C" );
//    Function<int, int> f( A, B );
//    f.Define_Mapping( 1, 10 );
//    f.Define_Mapping( 2, 20 );
//
//    Function<int, int> g( B, C );
//    g.Define_Mapping( 10, 100 );
//    g.Define_Mapping( 20, 200 );
//
//    auto composed = f.Define_Composition( g );
//
//    auto result1 = composed.Evaluate_Function( 1 );
//    TEST_ASSERT( result1.has_value() && result1.value() == 100, "(g∘f)(1) = 100" );
//
//    auto result2 = composed.Evaluate_Function( 2 );
//    TEST_ASSERT( result2.has_value() && result2.value() == 200, "(g∘f)(2) = 200" );
//}
//
//// ============================================================================
//// TEST 14: Function - Inverse
//// ============================================================================
//void test_function_inverse()
//{
//    TEST_START( "Function - Inverse" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//
//    TEST_SECTION( "Bijective Function Has Inverse" );
//    Function<int, int> bijective( domain, codomain );
//    bijective.Define_Mapping( 1, 10 );
//    bijective.Define_Mapping( 2, 20 );
//    bijective.Define_Mapping( 3, 30 );
//
//    auto inverse_opt = bijective.Define_Inverse();
//
//    TEST_ASSERT( inverse_opt.has_value(), "Inverse exists for bijective function" );
//
//    if ( inverse_opt.has_value() )
//    {
//        auto inverse = inverse_opt.value();
//
//        auto inv1 = inverse.Evaluate_Function( 10 );
//        TEST_ASSERT( inv1.has_value() && inv1.value() == 1, "f⁻¹(10) = 1" );
//
//        auto inv2 = inverse.Evaluate_Function( 20 );
//        TEST_ASSERT( inv2.has_value() && inv2.value() == 2, "f⁻¹(20) = 2" );
//
//        auto inv3 = inverse.Evaluate_Function( 30 );
//        TEST_ASSERT( inv3.has_value() && inv3.value() == 3, "f⁻¹(30) = 3" );
//    }
//
//    TEST_SECTION( "Non-Bijective Has No Inverse" );
//    Function<int, int> non_bijective( domain, codomain );
//    non_bijective.Define_Mapping( 1, 10 );
//    non_bijective.Define_Mapping( 2, 10 );  // Not injective
//
//    auto no_inverse = non_bijective.Define_Inverse();
//    TEST_ASSERT( !no_inverse.has_value(), "No inverse for non-bijective" );
//}
//
//// ============================================================================
//// TEST 15: Function - String to Int Mapping
//// ============================================================================
//void test_function_string_to_int()
//{
//    TEST_START( "Function - String to Int" );
//
//    Math_Set<std::string> names;
//    names.Insert_Element( "Alice" );
//    names.Insert_Element( "Bob" );
//    names.Insert_Element( "Charlie" );
//
//    Math_Set<int> ages;
//    ages.Insert_Element( 25 );
//    ages.Insert_Element( 30 );
//    ages.Insert_Element( 35 );
//
//    TEST_SECTION( "Age Mapping" );
//    Function<std::string, int> age_map( names, ages );
//    age_map.Define_Mapping( "Alice", 25 );
//    age_map.Define_Mapping( "Bob", 30 );
//    age_map.Define_Mapping( "Charlie", 35 );
//
//    auto alice_age = age_map.Evaluate_Function( "Alice" );
//    TEST_ASSERT( alice_age.has_value() && alice_age.value() == 25, "Alice is 25" );
//
//    auto bob_age = age_map.Evaluate_Function( "Bob" );
//    TEST_ASSERT( bob_age.has_value() && bob_age.value() == 30, "Bob is 30" );
//
//    TEST_ASSERT( age_map.Is_Bijective(), "Age mapping is bijective" );
//}
//
//// ============================================================================
//// TEST 16: Function - Images and Preimages
//// ============================================================================
//void test_function_images()
//{
//    TEST_START( "Function - Images and Preimages" );
//
//    Math_Set<int> domain;
//    for ( int i = 1; i <= 5; i++ )
//    {
//        domain.Insert_Element( i );
//    }
//
//    Math_Set<int> codomain;
//    for ( int i = 10; i <= 40; i += 10 )
//    {
//        codomain.Insert_Element( i );
//    }
//
//    TEST_SECTION( "Get Images (Range)" );
//    Function<int, int> f( domain, codomain );
//    f.Define_Mapping( 1, 10 );
//    f.Define_Mapping( 2, 20 );
//    f.Define_Mapping( 3, 20 );  // Duplicate output
//    // 4 and 5 not defined
//
//    auto images = f.Get_Images();
//
//    TEST_ASSERT( images.Set_Cardinality() == 2, "Two distinct outputs" );
//    TEST_ASSERT( images [ 0 ] == 10, "Image contains 10" );
//    TEST_ASSERT( images [ 1 ] == 20, "Image contains 20" );
//
//    TEST_SECTION( "Get Preimages (Defined Domain)" );
//    auto preimages = f.Get_PreImages();
//
//    TEST_ASSERT( preimages.Set_Cardinality() == 3, "Three inputs defined" );
//    TEST_ASSERT( preimages [ 0 ] == 1, "Preimage contains 1" );
//    TEST_ASSERT( preimages [ 1 ] == 2, "Preimage contains 2" );
//    TEST_ASSERT( preimages [ 2 ] == 3, "Preimage contains 3" );
//}
//
//// ============================================================================
//// TEST 17: Edge Cases - Empty Relations
//// ============================================================================
//void test_edge_cases_relations()
//{
//    TEST_START( "Edge Cases - Relations" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//
//    TEST_SECTION( "Empty Relation Properties" );
//    Relation<int> empty_rel( domain );
//
//    TEST_ASSERT( !empty_rel.Is_Reflexive(), "Empty not reflexive" );
//    TEST_ASSERT( empty_rel.Is_Symmetric(), "Empty is vacuously symmetric" );
//    TEST_ASSERT( empty_rel.Is_AntiSymmetric(), "Empty is vacuously antisymmetric" );
//    TEST_ASSERT( empty_rel.Is_Transitive(), "Empty is vacuously transitive" );
//
//    TEST_SECTION( "Full Relation (All Pairs)" );
//    Relation<int> full_rel( domain );
//    full_rel.Add_Pair( 1, 1 );
//    full_rel.Add_Pair( 1, 2 );
//    full_rel.Add_Pair( 2, 1 );
//    full_rel.Add_Pair( 2, 2 );
//
//    TEST_ASSERT( full_rel.Is_Reflexive(), "Full relation is reflexive" );
//    TEST_ASSERT( full_rel.Is_Symmetric(), "Full relation is symmetric" );
//    TEST_ASSERT( !full_rel.Is_AntiSymmetric(), "Full relation not antisymmetric" );
//    TEST_ASSERT( full_rel.Is_Transitive(), "Full relation is transitive" );
//}
//
//// ============================================================================
//// TEST 18: Edge Cases - Empty Functions
//// ============================================================================
//void test_edge_cases_functions()
//{
//    TEST_START( "Edge Cases - Functions" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//
//    TEST_SECTION( "Partially Defined Function" );
//    Function<int, int> partial( domain, codomain );
//    partial.Define_Mapping( 1, 10 );
//    // 2 and 3 not defined - NOT a proper function!
//
//    TEST_ASSERT( partial.Is_Defined_At( 1 ), "1 is defined" );
//    TEST_ASSERT( !partial.Is_Defined_At( 2 ), "2 is not defined" );
//    TEST_ASSERT( !partial.Is_Defined_At( 3 ), "3 is not defined" );
//    TEST_ASSERT( !partial.Is_Proper_Function(), "Not fully defined on domain" );
//
//    // Partial functions are NOT injective/surjective (not proper functions)
//    TEST_ASSERT( !partial.Is_Injective(), "Partial function not injective" );
//    TEST_ASSERT( !partial.Is_Surjective(), "Partial function not surjective" );
//
//    TEST_SECTION( "Empty Function (No Mappings)" );
//    Function<int, int> empty_func( domain, codomain );
//
//    TEST_ASSERT( !empty_func.Is_Defined_At( 1 ), "No mappings defined" );
//    TEST_ASSERT( !empty_func.Is_Proper_Function(), "Empty function not proper" );
//    // Empty/partial functions fail injective/surjective tests
//    TEST_ASSERT( !empty_func.Is_Injective(), "Empty function not injective" );
//    TEST_ASSERT( !empty_func.Is_Surjective(), "Empty function not surjective" );
//
//    auto images = empty_func.Get_Images();
//    TEST_ASSERT( images.Set_Cardinality() == 0, "No images for empty function" );
//
//    auto preimages = empty_func.Get_PreImages();
//    TEST_ASSERT( preimages.Set_Cardinality() == 0, "No preimages for empty function" );
//}
//
//// ============================================================================
//// TEST 19: Large Domain Relation Tests
//// ============================================================================
//void test_large_domain_relations()
//{
//    TEST_START( "Large Domain Relations" );
//
//    Math_Set<int> large_domain;
//    for ( int i = 1; i <= 10; i++ )
//    {
//        large_domain.Insert_Element( i );
//    }
//
//    TEST_SECTION( "Large Reflexive Relation" );
//    Relation<int> large_reflexive( large_domain );
//    for ( int i = 1; i <= 10; i++ )
//    {
//        large_reflexive.Add_Pair( i, i );
//    }
//
//    TEST_ASSERT( large_reflexive.Is_Reflexive(), "All 10 elements have self-loops" );
//    TEST_ASSERT( large_reflexive.Get_Domain_Size() == 10, "Domain size is 10" );
//    TEST_ASSERT( large_reflexive.Get_Relation_Size() == 10, "10 pairs in relation" );
//
//    TEST_SECTION( "Chain Relation (Transitive Test)" );
//    Relation<int> chain( large_domain );
//    // Create chain: 1→2→3→4→5
//    for ( int i = 1; i < 5; i++ )
//    {
//        chain.Add_Pair( i, i + 1 );
//    }
//
//    TEST_ASSERT( !chain.Is_Transitive(), "Chain without closure not transitive" );
//
//    auto transitive_chain = chain.Transitive_Closure();
//    TEST_ASSERT( transitive_chain.Is_Transitive(), "Transitive closure is transitive" );
//    TEST_ASSERT( transitive_chain.Contains_Pair( 1, 3 ), "1→3 added by transitivity" );
//    TEST_ASSERT( transitive_chain.Contains_Pair( 1, 4 ), "1→4 added by transitivity" );
//    TEST_ASSERT( transitive_chain.Contains_Pair( 1, 5 ), "1→5 added by transitivity" );
//    TEST_ASSERT( transitive_chain.Contains_Pair( 2, 4 ), "2→4 added by transitivity" );
//    TEST_ASSERT( transitive_chain.Contains_Pair( 2, 5 ), "2→5 added by transitivity" );
//
//    TEST_SECTION( "Complete Graph Relation" );
//    Relation<int> complete( large_domain );
//    for ( int i = 1; i <= 10; i++ )
//    {
//        for ( int j = 1; j <= 10; j++ )
//        {
//            complete.Add_Pair( i, j );
//        }
//    }
//
//    TEST_ASSERT( complete.Is_Reflexive(), "Complete graph is reflexive" );
//    TEST_ASSERT( complete.Is_Symmetric(), "Complete graph is symmetric" );
//    TEST_ASSERT( complete.Is_Transitive(), "Complete graph is transitive" );
//    TEST_ASSERT( complete.Is_Equivalence_Relation(), "Complete graph is equivalence" );
//    TEST_ASSERT( complete.Get_Relation_Size() == 100, "10×10 = 100 pairs" );
//}
//
//// ============================================================================
//// TEST 20: String Domain Advanced Tests
//// ============================================================================
//void test_string_domain_advanced()
//{
//    TEST_START( "String Domain - Advanced" );
//
//    Math_Set<std::string> cities;
//    cities.Insert_Element( "New York" );
//    cities.Insert_Element( "Los Angeles" );
//    cities.Insert_Element( "Chicago" );
//    cities.Insert_Element( "Houston" );
//    cities.Insert_Element( "Phoenix" );
//
//    TEST_SECTION( "Flight Connection Relation" );
//    Relation<std::string> flights( cities );
//    flights.Add_Pair( "New York", "Los Angeles" );
//    flights.Add_Pair( "Los Angeles", "New York" );
//    flights.Add_Pair( "New York", "Chicago" );
//    flights.Add_Pair( "Chicago", "New York" );
//    flights.Add_Pair( "Chicago", "Houston" );
//    flights.Add_Pair( "Houston", "Chicago" );
//    flights.Add_Pair( "Los Angeles", "Phoenix" );
//    flights.Add_Pair( "Phoenix", "Los Angeles" );
//
//    TEST_ASSERT( flights.Is_Symmetric(), "Flights are bidirectional" );
//    TEST_ASSERT( !flights.Is_Reflexive(), "Cities don't connect to themselves" );
//    TEST_ASSERT( !flights.Is_Transitive(), "Not all indirect connections available" );
//
//    TEST_SECTION( "City Distance Function" );
//    Math_Set<std::string> src_cities;
//    src_cities.Insert_Element( "New York" );
//    src_cities.Insert_Element( "Los Angeles" );
//    src_cities.Insert_Element( "Chicago" );
//
//    Math_Set<int> distances;
//    distances.Insert_Element( 0 );
//    distances.Insert_Element( 1000 );
//    distances.Insert_Element( 2000 );
//    distances.Insert_Element( 2800 );
//
//    Function<std::string, int> distance_from_ny( src_cities, distances );
//    distance_from_ny.Define_Mapping( "New York", 0 );
//    distance_from_ny.Define_Mapping( "Chicago", 1000 );
//    distance_from_ny.Define_Mapping( "Los Angeles", 2800 );
//
//    auto ny_dist = distance_from_ny.Evaluate_Function( "New York" );
//    TEST_ASSERT( ny_dist.has_value() && ny_dist.value() == 0, "NY to NY = 0 miles" );
//
//    auto chi_dist = distance_from_ny.Evaluate_Function( "Chicago" );
//    TEST_ASSERT( chi_dist.has_value() && chi_dist.value() == 1000, "NY to Chicago = 1000 miles" );
//
//    auto la_dist = distance_from_ny.Evaluate_Function( "Los Angeles" );
//    TEST_ASSERT( la_dist.has_value() && la_dist.value() == 2800, "NY to LA = 2800 miles" );
//
//    TEST_ASSERT( distance_from_ny.Is_Injective(), "Each city at unique distance" );
//}
//
//// ============================================================================
//// TEST 21: Function Composition Chain
//// ============================================================================
//void test_function_composition_chain()
//{
//    TEST_START( "Function Composition - Chain" );
//
//    Math_Set<int> A, B, C, D;
//    A.Insert_Element( 1 );
//    A.Insert_Element( 2 );
//
//    B.Insert_Element( 10 );
//    B.Insert_Element( 20 );
//
//    C.Insert_Element( 100 );
//    C.Insert_Element( 200 );
//
//    D.Insert_Element( 1000 );
//    D.Insert_Element( 2000 );
//
//    TEST_SECTION( "Three-Function Composition" );
//    Function<int, int> f( A, B );
//    f.Define_Mapping( 1, 10 );
//    f.Define_Mapping( 2, 20 );
//
//    Function<int, int> g( B, C );
//    g.Define_Mapping( 10, 100 );
//    g.Define_Mapping( 20, 200 );
//
//    Function<int, int> h( C, D );
//    h.Define_Mapping( 100, 1000 );
//    h.Define_Mapping( 200, 2000 );
//
//    // Compose: h ∘ g ∘ f
//    auto gf = f.Define_Composition( g );
//    auto hgf = gf.Define_Composition( h );
//
//    auto result1 = hgf.Evaluate_Function( 1 );
//    TEST_ASSERT( result1.has_value() && result1.value() == 1000, "(h∘g∘f)(1) = 1000" );
//
//    auto result2 = hgf.Evaluate_Function( 2 );
//    TEST_ASSERT( result2.has_value() && result2.value() == 2000, "(h∘g∘f)(2) = 2000" );
//
//    TEST_ASSERT( hgf.Is_Bijective(), "Composition of bijections is bijective" );
//}
//
//// ============================================================================
//// TEST 22: Inverse Function Properties
//// ============================================================================
//void test_inverse_function_properties()
//{
//    TEST_START( "Inverse Function - Properties" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    Math_Set<int> codomain;
//    codomain.Insert_Element( 10 );
//    codomain.Insert_Element( 20 );
//    codomain.Insert_Element( 30 );
//
//    TEST_SECTION( "f ∘ f⁻¹ = Identity" );
//    Function<int, int> f( domain, codomain );
//    f.Define_Mapping( 1, 10 );
//    f.Define_Mapping( 2, 20 );
//    f.Define_Mapping( 3, 30 );
//
//    auto inv = f.Define_Inverse();
//    TEST_ASSERT( inv.has_value(), "Inverse exists" );
//
//    if ( inv.has_value() )
//    {
//        auto f_inv = inv.value();
//
//        // Compose f with its inverse
//        auto identity = f_inv.Define_Composition( f );
//
//        auto id1 = identity.Evaluate_Function( 10 );
//        TEST_ASSERT( id1.has_value() && id1.value() == 10, "f⁻¹(f(10)) = 10" );
//
//        auto id2 = identity.Evaluate_Function( 20 );
//        TEST_ASSERT( id2.has_value() && id2.value() == 20, "f⁻¹(f(20)) = 20" );
//
//        auto id3 = identity.Evaluate_Function( 30 );
//        TEST_ASSERT( id3.has_value() && id3.value() == 30, "f⁻¹(f(30)) = 30" );
//
//        TEST_SECTION( "Inverse of Inverse" );
//        auto inv_inv = f_inv.Define_Inverse();
//        TEST_ASSERT( inv_inv.has_value(), "Inverse of inverse exists" );
//
//        if ( inv_inv.has_value() )
//        {
//            auto f_restored = inv_inv.value();
//
//            auto orig1 = f_restored.Evaluate_Function( 1 );
//            TEST_ASSERT( orig1.has_value() && orig1.value() == 10, "(f⁻¹)⁻¹ = f" );
//        }
//    }
//}
//
//// ============================================================================
//// TEST 23: Relation Closure Combinations
//// ============================================================================
//void test_closure_combinations()
//{
//    TEST_START( "Relation Closures - Combinations" );
//
//    Math_Set<int> domain;
//    domain.Insert_Element( 1 );
//    domain.Insert_Element( 2 );
//    domain.Insert_Element( 3 );
//
//    TEST_SECTION( "Reflexive + Symmetric Closure" );
//    Relation<int> rel( domain );
//    rel.Add_Pair( 1, 2 );
//
//    auto reflexive = rel.Reflexive_Closure();
//    auto ref_sym = reflexive.Symmetric_Closure();
//
//    TEST_ASSERT( ref_sym.Is_Reflexive(), "Has all self-loops" );
//    TEST_ASSERT( ref_sym.Is_Symmetric(), "All pairs are symmetric" );
//    TEST_ASSERT( ref_sym.Contains_Pair( 1, 2 ), "Original pair preserved" );
//    TEST_ASSERT( ref_sym.Contains_Pair( 2, 1 ), "Symmetric pair added" );
//    TEST_ASSERT( ref_sym.Contains_Pair( 1, 1 ), "Self-loop added" );
//
//    TEST_SECTION( "All Three Closures → Equivalence" );
//    Relation<int> sparse( domain );
//    sparse.Add_Pair( 1, 2 );
//    sparse.Add_Pair( 2, 3 );
//
//    auto step1 = sparse.Reflexive_Closure();
//    auto step2 = step1.Symmetric_Closure();
//    auto equiv = step2.Transitive_Closure();
//
//    TEST_ASSERT( equiv.Is_Equivalence_Relation(), "Triple closure produces equivalence" );
//    TEST_ASSERT( equiv.Contains_Pair( 1, 3 ), "Transitivity added" );
//    TEST_ASSERT( equiv.Contains_Pair( 3, 1 ), "Symmetry of transitivity added" );
//}
//
//// ============================================================================
//// TEST 24: Mixed Type Functions
//// ============================================================================
//void test_mixed_type_functions()
//{
//    TEST_START( "Functions - Mixed Types" );
//
//    Math_Set<std::string> names;
//    names.Insert_Element( "Alice" );
//    names.Insert_Element( "Bob" );
//    names.Insert_Element( "Charlie" );
//
//    Math_Set<int> ids;
//    ids.Insert_Element( 101 );
//    ids.Insert_Element( 102 );
//    ids.Insert_Element( 103 );
//
//    Math_Set<std::string> departments;
//    departments.Insert_Element( "Engineering" );
//    departments.Insert_Element( "Sales" );
//    departments.Insert_Element( "Marketing" );
//
//    TEST_SECTION( "Name → ID → Department" );
//    Function<std::string, int> name_to_id( names, ids );
//    name_to_id.Define_Mapping( "Alice", 101 );
//    name_to_id.Define_Mapping( "Bob", 102 );
//    name_to_id.Define_Mapping( "Charlie", 103 );
//
//    Function<int, std::string> id_to_dept( ids, departments );
//    id_to_dept.Define_Mapping( 101, "Engineering" );
//    id_to_dept.Define_Mapping( 102, "Sales" );
//    id_to_dept.Define_Mapping( 103, "Marketing" );
//
//    auto name_to_dept = name_to_id.Define_Composition( id_to_dept );
//
//    auto alice_dept = name_to_dept.Evaluate_Function( "Alice" );
//    TEST_ASSERT( alice_dept.has_value() && alice_dept.value() == "Engineering",
//                 "Alice → Engineering" );
//
//    auto bob_dept = name_to_dept.Evaluate_Function( "Bob" );
//    TEST_ASSERT( bob_dept.has_value() && bob_dept.value() == "Sales",
//                 "Bob → Sales" );
//
//    TEST_ASSERT( name_to_dept.Is_Bijective(), "Name-to-department is bijective" );
//}
//
//// ============================================================================
//// TEST 25: Stress Test - Performance
//// ============================================================================
//void test_performance_stress()
//{
//    TEST_START( "Performance - Stress Test" );
//
//    Math_Set<int> large_domain;
//    for ( int i = 0; i < 50; i++ )
//    {
//        large_domain.Insert_Element( i );
//    }
//
//    TEST_SECTION( "Large Transitive Closure" );
//    Relation<int> large_chain( large_domain );
//
//    // Create a long chain
//    for ( int i = 0; i < 49; i++ )
//    {
//        large_chain.Add_Pair( i, i + 1 );
//    }
//
//    auto closure = large_chain.Transitive_Closure();
//
//    TEST_ASSERT( closure.Is_Transitive(), "Large transitive closure computed" );
//    TEST_ASSERT( closure.Contains_Pair( 0, 49 ), "End-to-end path exists" );
//    TEST_ASSERT( closure.Contains_Pair( 0, 25 ), "Mid-point path exists" );
//
//    TEST_SECTION( "Large Bijective Function" );
//    Math_Set<int> codomain;
//    for ( int i = 100; i < 150; i++ )
//    {
//        codomain.Insert_Element( i );
//    }
//
//    Function<int, int> large_func( large_domain, codomain );
//    for ( int i = 0; i < 50; i++ )
//    {
//        large_func.Define_Mapping( i, 100 + i );
//    }
//
//    TEST_ASSERT( large_func.Is_Bijective(), "Large function is bijective" );
//
//    auto inv = large_func.Define_Inverse();
//    TEST_ASSERT( inv.has_value(), "Large function has inverse" );
//}
//
//// ============================================================================
//// MAIN FUNCTION
//// ============================================================================
//int main()
//{
//    std::cout << BOLD << BLUE << "\n";
//    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
//    std::cout << "║     COMPREHENSIVE RELATIONS & FUNCTIONS TEST SUITE        ║\n";
//    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
//    std::cout << RESET << std::endl;
//
//    // Relation Tests
//    test_relation_reflexive();
//    test_relation_symmetric();
//    test_relation_antisymmetric();
//    test_relation_transitive();
//    test_equivalence_relations();
//    test_partial_orders();
//    test_relation_closures();
//    test_relation_strings();
//
//    // Function Tests
//    test_function_basic();
//    test_function_injective();
//    test_function_surjective();
//    test_function_bijective();
//    test_function_composition();
//    test_function_inverse();
//    test_function_string_to_int();
//    test_function_images();
//
//    // Edge Cases & Advanced Tests
//    test_edge_cases_relations();
//    test_edge_cases_functions();
//    test_large_domain_relations();
//    test_string_domain_advanced();
//    test_function_composition_chain();
//    test_inverse_function_properties();
//    test_closure_combinations();
//    test_mixed_type_functions();
//    test_performance_stress();
//
//    // Print final summary
//    print_summary();
//
//    return ( failed_tests == 0 ) ? 0 : 1;
//}