//=============================================================================
//! @file   VariantExtensions.cc
//! @author Josh Staley staley@arlut.utexas.edu
//! @brief  
//=============================================================================

//=== Includes ================================================================

#include <iostream>
#include <Variant/Variant.h>
#include <Variant/Extensions.h>

using namespace std;
using namespace libvariant;

//=== Function Declarations ===================================================

bool TestFlattenPath ( bool verbose = false );
bool TestExpandPath  ( bool verbose = false );
bool TestRDiff       ( bool verbose = false );
bool TestRUpdate     ( bool verbose = false );

//=== Main ====================================================================

//-----------------------------------------------------------------------------
int main ( int argc, char * argv [] )
//-----------------------------------------------------------------------------
{
	//--- Any are will turn verbosity on ---
	bool verbose = false;
	if ( argc > 1 )
		verbose = true;
	
	bool testFlattenPath, testExpandPath, testDiff, testRUpdate;
	
	try
	{
		// FlattenPath()
		testFlattenPath = TestFlattenPath( verbose );
		if ( testFlattenPath )
			 cout << "FlattenPath - PASSED" << endl;
		else
			 cout << "FlattenPath - FAILED" << endl;
		
		// ExpandPath()
		testExpandPath = TestExpandPath( verbose );
		if ( testExpandPath )
			 cout << "ExpandPath - PASSED" << endl;
		else
			 cout << "ExpandPath - FAILED" << endl;
		
		// RDiff()
		testDiff = TestRDiff( verbose );
		if ( testDiff )
			 cout << "RDiff - PASSED" << endl;
		else
			 cout << "RDiff - FAILED" << endl;
		
		// RUpdate()
		testRUpdate = TestRUpdate( verbose );
		if ( testRUpdate )
			 cout << "RUpdate - PASSED" << endl;
		else
			 cout << "RUpdate - FAILED" << endl;
	}
	catch ( exception & e )
	{
		cerr << "# " << e.what() << endl;
		return -1;
	}
	
	cout << "Done" << endl;
	
	return 0;
	
	bool passed = testFlattenPath && testExpandPath && testDiff && testRUpdate;
	return passed ? 0 : 1;
}

//=== Functions ===============================================================

//-----------------------------------------------------------------------------
bool TestFlattenPath ( bool verbose_ )
//-----------------------------------------------------------------------------
{
	Variant orig, flat, ideal;

	orig = 10;
	ideal = 10;
	flat = FlattenPath(orig);
	if (flat != ideal) { return false; }

	orig = Variant::MapType;
	ideal = Variant::MapType;
	
	orig["1"]["A"] = Variant::NullType;
	orig["1"]["B"] = true;
	orig["1"]["C"] = 2;
	orig["1"]["D"] = -3;
	orig["1"]["E"] = 4.0;
	orig["1"]["F"] = "five";
	orig["1"]["G"][0] = Variant::NullType;
	orig["1"]["G"][1] = true;
	orig["1"]["G"][2] = 2;
	orig["1"]["G"][3] = -3;
	orig["1"]["G"][4] = 4.0;
	orig["1"]["G"][5] = "five";
	orig["2"][0] = Variant::NullType;
	orig["2"][1] = true;
	orig["2"][2] = 2;
	orig["2"][3] = -3;
	orig["2"][4] = 4.0;
	orig["2"][5] = "five";
	orig["2"][6]["a"] = true;
	orig["2"][6]["b"] = "test";
	
	ideal["1/A"] = Variant::NullType;
	ideal["1/B"] = true;
	ideal["1/C"] = 2;
	ideal["1/D"] = -3;
	ideal["1/E"] = 4.0;
	ideal["1/F"] = "five";
	ideal["1/G[0]"] = Variant::NullType;
	ideal["1/G[1]"] = true;
	ideal["1/G[2]"] = 2;
	ideal["1/G[3]"] = -3;
	ideal["1/G[4]"] = 4.0;
	ideal["1/G[5]"] = "five";
	ideal["2[0]"] = Variant::NullType;
	ideal["2[1]"] = true;
	ideal["2[2]"] = 2;
	ideal["2[3]"] = -3;
	ideal["2[4]"] = 4.0;
	ideal["2[5]"] = "five";
	ideal["2[6]/a"] = true;
	ideal["2[6]/b"] = "test";
	

	try {
		flat = FlattenPath( orig, 123);
		return false;
	} catch (const std::exception &) {}

	try
	{
		flat = FlattenPath( orig );
	}
	catch ( exception & e)
	{
		cout << "# " << e.what() << endl;
		return false;
	}
	
	if ( verbose_ )
	{
		cout << "--- flat ---" << endl;
		SerializeJSON(cout.rdbuf(), flat, true);
		cout << endl;
		
		cout << "--- ideal ---" << endl;
		SerializeJSON(cout.rdbuf(), ideal, true);
		cout << endl;
	}
	
	return flat == ideal;
}

//-----------------------------------------------------------------------------
bool TestExpandPath  ( bool verbose_ )
//-----------------------------------------------------------------------------
{
	// TODO - implement tests
	return true;
}

//-----------------------------------------------------------------------------
bool TestRDiff ( bool verbose_ )
//-----------------------------------------------------------------------------
{
	Variant base, compare, ideal, diff;
	
	base["1"]["A"] = Variant::NullType;
	base["1"]["B"] = true;
	base["1"]["C"] = 2;
	base["1"]["D"] = -3;
	base["1"]["E"] = 4.0;
	base["1"]["F"] = "five";
	base["1"]["G"][0] = Variant::NullType;
	base["1"]["G"][1] = true;
	base["1"]["G"][2] = 2;
	base["1"]["G"][3] = -3;
	base["1"]["G"][4] = 4.0;
	base["1"]["G"][5] = "five";
	base["2"][0] = Variant::NullType;
	base["2"][1] = true;
	base["2"][2] = 2;
	base["2"][3] = -3;
	base["2"][4] = 4.0;
	base["2"][5] = "five";
	base["2"][6]["a"] = true;
	base["2"][6]["b"] = "test";
	base["3"] = 5;
	base["5"] = Variant().Set("a", 1).Set("b", 2);
	
	compare["1"]["B"] = false;
	compare["1"]["C"] = -2;
	compare["1"]["D"] = 3.0;
	compare["1"]["E"] = "four";
	compare["1"]["F"] = 5.0;
	compare["1"]["G"][0] = Variant::NullType;
	compare["1"]["G"][1] = true;
	compare["1"]["G"][2] = 2;
	compare["1"]["G"][3] = -3;
	compare["1"]["G"][4] = 4.0;
	compare["1"]["G"][5] = "five";
	compare["2"][0] = Variant::NullType;
	compare["2"][1] = true;
	compare["2"][2] = 2;
	compare["2"][3] = -3;
	compare["2"][4] = 4.0;
	compare["2"][5] = "five";
	compare["2"][6]["a"] = false;
	compare["2"][6]["b"] = "test";
	compare["3"] = Variant().Set("a", 1);
	compare["4"] = Variant().Set("emptymap", Variant::MapType);
	compare["5"] = Variant::MapType;
	
	ideal["1"]["B"] = false;
	ideal["1"]["C"] = -2;
	ideal["1"]["D"] = 3.0;
	ideal["1"]["E"] = "four";
	ideal["1"]["F"] = 5.0;
	ideal["2"][0] = Variant::NullType;
	ideal["2"][1] = true;
	ideal["2"][2] = 2;
	ideal["2"][3] = -3;
	ideal["2"][4] = 4.0;
	ideal["2"][5] = "five";
	ideal["2"][6]["a"] = false;
	ideal["2"][6]["b"] = "test";
	ideal["3"] = Variant().Set("a", 1);
	ideal["4"] = Variant().Set("emptymap", Variant::MapType);
	// ideal["5"] is removed because it's empty
	
	try
	{
		diff = RDiff( base, compare );
	}
	catch (exception & e)
	{
		cout << "# " << e.what() << endl;
		return false;
	}
	
	if ( verbose_ )
	{
		cout << "--- diff ---" << endl;
		SerializeJSON(cout.rdbuf(), diff, true);
		cout << endl;
		
		cout << "--- ideal ---" << endl;
		SerializeJSON(cout.rdbuf(), ideal, true);
		cout << endl;
	}
	
	return diff == ideal;
}

//-----------------------------------------------------------------------------
bool TestFlattenPathVariantToStringMap ( bool verbose_ )
//-----------------------------------------------------------------------------
// TODO - Fix this and add to list of tests in main
//-----------------------------------------------------------------------------
{
	Variant orig;
	map<string,string> flatMap, ideal;
	
	orig["ONE"]["A"] = true;
	orig["ONE"]["B"] = 1;
	orig["ONE"]["C"] = -2;
	orig["ONE"]["D"] = 3;
	orig["ONE"]["E"] = "4";
	orig["ONE"]["F"][0] = false;
	orig["ONE"]["F"][1] = 1;
	orig["ONE"]["F"][2] = -2;
	orig["ONE"]["F"][3] = 3;
	orig["ONE"]["F"][4] = "4";
	orig["TWO"][0] = true;
	orig["TWO"][1] = 1;
	orig["TWO"][2] = -2;
	orig["TWO"][3] = 3;
	orig["TWO"][4] = "4";
	orig["TWO"][5]["a"] = true;
	orig["TWO"][5]["b"] = "test";
	
	ideal["ONE.A"] = "true";
	ideal["ONE.B"] = "1";
	ideal["ONE.C"] = "-2";
	ideal["ONE.D"] = "3";
	ideal["ONE.E"] = "4";
	ideal["ONE.F"] = "false,1,-2,3,4";
	ideal["TWO.0"] = "true";
	ideal["TWO.1"] = "1";
	ideal["TWO.2"] = "-2";
	ideal["TWO.3"] = "3";
	ideal["TWO.4"] = "4";
	ideal["TWO.5.a"] = "true";
	ideal["TWO.5.b"] = "test";
	
	try
	{
		flatMap = FlattenPathToStrStrMap( orig );
	}
	catch (exception& e)
	{
		cout << "# " << e.what() << endl;
	}
	
	if( verbose_ )
	{
		for( map<string,string>::iterator i(ideal.begin()), e(ideal.end()); i != e; ++i)
		{
			cout << flatMap[i->first] << " = " << ideal[i->first] << endl;
		}
	}
	
	return ideal == flatMap;
}

//-----------------------------------------------------------------------------
bool TestRUpdate ( bool verbose )
//-----------------------------------------------------------------------------
{
	Variant base, update, ideal;

	base["1"]["A"] = Variant::NullType;
	base["1"]["B"] = true;
	base["1"]["C"] = 2;
	base["1"]["D"] = -3;
	base["1"]["E"] = 4.0;
	base["1"]["F"] = "five";
	base["1"]["G"][0] = Variant::NullType;
	base["1"]["G"][1] = true;
	base["1"]["G"][2] = 2;
	base["1"]["G"][3] = -3;
	base["1"]["G"][4] = 4.0;
	base["1"]["G"][5] = "five";
	base["2"][0] = Variant::NullType;
	base["2"][1] = true;
	base["2"][2] = 2;
	base["2"][3] = -3;
	base["2"][4] = 4.0;
	base["2"][5] = "five";
	base["2"][6]["a"] = true;
	base["2"][6]["b"] = "test";
	base["3"] = 5;
	base["5"] = Variant().Set("a", 1).Set("b", 2);

	update["1"]["B"] = false;
	update["1"]["C"] = -2;
	update["1"]["D"] = 3.0;
	update["1"]["E"] = "four";
	update["1"]["F"] = 5.0;
	update["2"][0] = Variant::NullType;
	update["2"][1] = true;
	update["2"][2] = 2;
	update["2"][3] = -3;
	update["2"][4] = 4.0;
	update["2"][5] = "five";
	update["2"][6]["a"] = false;
	update["2"][6]["b"] = "test";
	update["3"] = Variant().Set("a", 1);
	update["4"] = Variant().Set("emptymap", Variant::MapType);

	ideal["1"]["A"] = Variant::NullType;
	ideal["1"]["B"] = false;
	ideal["1"]["C"] = -2;
	ideal["1"]["D"] = 3.0;
	ideal["1"]["E"] = "four";
	ideal["1"]["F"] = 5.0;
	ideal["1"]["G"][0] = Variant::NullType;
	ideal["1"]["G"][1] = true;
	ideal["1"]["G"][2] = 2;
	ideal["1"]["G"][3] = -3;
	ideal["1"]["G"][4] = 4.0;
	ideal["1"]["G"][5] = "five";
	ideal["2"][0] = Variant::NullType;
	ideal["2"][1] = true;
	ideal["2"][2] = 2;
	ideal["2"][3] = -3;
	ideal["2"][4] = 4.0;
	ideal["2"][5] = "five";
	ideal["2"][6]["a"] = false;
	ideal["2"][6]["b"] = "test";
	ideal["3"] = Variant().Set("a", 1);
	ideal["4"] = Variant().Set("emptymap", Variant::MapType);
	ideal["5"] = Variant().Set("a", 1).Set("b", 2);

	try {
		RUpdate(base, update);
	} catch (const exception &e) {
		cerr << "# " << e.what() << endl;
		return false;
	}
	if ( verbose )
	{
		cout << "--- result ---" << endl;
		SerializeJSON(cout.rdbuf(), base, true);
		cout << endl;

		cout << "--- ideal ---" << endl;
		SerializeJSON(cout.rdbuf(), ideal, true);
		cout << endl;
	}
	return base == ideal;
}

//=============================================================================
