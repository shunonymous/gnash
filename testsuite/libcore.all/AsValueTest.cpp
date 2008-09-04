// 
//   Copyright (C) 2008 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#ifdef HAVE_DEJAGNU_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <log.h>
#include <iostream>
#include <string>
#include "VM.h"
#include "DummyMovieDefinition.h"
#include "DummyCharacter.h"
#include "ManualClock.h"
#include "movie_definition.h"
#include "dejagnu.h"
#include "as_value.h"
#include "as_object.h"
#include "arg_parser.h"
#include "buffer.h"
#include "network.h"
#include "amf.h"
#include "element.h"

using namespace amf;
using namespace gnash;
using namespace std;

static void usage (void);

// Prototypes for test cases
static void test_el();
static void test_obj();
static void test_isnan();

// Enable the display of memory allocation and timing data
static bool memdebug = false;

TestState runtest;
LogFile& dbglogfile = LogFile::getDefaultInstance();
RcInitFile& rcfile = RcInitFile::getDefaultInstance();

int
main(int argc, char *argv[])
{    const Arg_parser::Option opts[] =
        {
            { 'h', "help",          Arg_parser::no  },
            { 'v', "verbose",       Arg_parser::no  },
            { 'd', "dump",          Arg_parser::no  },
        };
    
    Arg_parser parser(argc, argv, opts);
    if( ! parser.error().empty() ) {
        cout << parser.error() << endl;
        exit(EXIT_FAILURE);
    }
    
    for( int i = 0; i < parser.arguments(); ++i ) {
        const int code = parser.code(i);
        try {
            switch( code ) {
              case 'h':
                  usage ();
                  exit(EXIT_SUCCESS);
              case 'v':
                    dbglogfile.setVerbosity();
                    // This happens once per 'v' flag 
                    log_debug(_("Verbose output turned on"));
                    break;
	    }
        }
        
        catch (Arg_parser::ArgParserException &e) {
            cerr << _("Error parsing command line options: ") << e.what() << endl;
            cerr << _("This is a Gnash bug.") << endl;
        }
    }

    // Initialize gnash lib
    gnashInit();

    // Create a bogus movie with swf version 7 support
    boost::intrusive_ptr<movie_definition> md ( new DummyMovieDefinition(7) );
    ManualClock clock;
    VM& vm = VM::init(*md, clock);
    
    movie_instance* root = md->create_movie_instance();
    vm.getRoot().setRootMovie(root);

    // run the tests
    test_isnan();
    test_el();
    test_obj();
}

void
test_el()
{
//    bool notest = false;
    
    Element el1;
    el1.makeNumber(1.234);
    as_value as1(el1);
    if (as1.to_number() == el1.to_number()) {
        runtest.pass("as_value(Element &number)");
    } else {
        runtest.fail("as_value(Element &number)");
    }
//     Element el11 = as1.to_element();
//     if (el11.to_number() == as1.to_number()) {
//         runtest.pass("as_value::to_element(number)");
//     } else {
//         runtest.fail("as_value::to_element(number)");
//     }
    
    Element el2;
    el2.makeString("Hello World");
    as_value as2(&el2);
    if (as2.to_string() == el2.to_string()) {
        runtest.pass("as_value(Element &string)");
    } else {
        runtest.fail("as_value(Element &string)");
    }
    
    Element el3;
    el3.makeBoolean(true);
    as_value as3(&el3);    
    if ((as3.is_bool()) && (as3.to_bool() == true)) {
        runtest.pass("as_value(Element &bool)");
    } else {
        runtest.fail("as_value(Element &bool)");
    }

    Element el4;
    el4.makeUndefined();
    as_value as4(&el4);
    if (as4.is_undefined()) {
        runtest.pass("as_value(Element &undefined)");
    } else {
        runtest.fail("as_value(Element &undefined)");
    }
    
    Element el5;
    el5.makeMovieClip();
    as_value as5(&el5);
    if (as5.is_sprite()) {
        runtest.pass("as_value(Element &movieclip)");
    } else {
        runtest.fail("as_value(Element &movieclip)");
    }

    // There is no equivalent AMF element type to the as_value AS_FUNCTION type
}

void
test_obj()
{
    // Create an object element with some properties
    bool notest = false;
    Element top;
    top.makeObject("toplevel");

    Element *foo = new Element;
    foo->makeString("foo", "bar");
    top.addProperty(foo);

    Element *bar = new Element;
    bar->makeNumber("bar", 1.234);
    top.addProperty(bar);

    if (top.propertySize() != 2) {
        notest = true;
    }
    
    VM& vm = VM::get();
    string_table& st = vm.getStringTable();

    as_value as1(&top);
    
    if (as1.is_object()) {
        runtest.pass("as_value(Element &object)");
    } else {
        notest = true;
        runtest.fail("as_value(Element &object)");
    }

    as_value fooas, baras;
    boost::intrusive_ptr<as_object> ao1 = as1.to_object();

    if (ao1 == 0) {
        notest= true;
    }
        
    if (notest) {
        runtest.unresolved("as_value(Element &prop1)");
    } else {
        if (ao1.get() == 0) {
            runtest.unresolved("as_value(Element &prop1)");
        } else {
            ao1.get()->get_member(st.find(PROPNAME("foo")), &fooas);
            ao1.get()->get_member(st.find(PROPNAME("bar")), &baras);
            if ((fooas.is_string()) && (fooas.to_string() == foo->to_string())) {
                runtest.pass("as_value(Element &->prop1)");
            } else {
                runtest.fail("as_value(Element &->prop1)");
            }
            if ((baras.is_number()) && (baras.to_number() == bar->to_number())) {
                runtest.pass("as_value(Element &->prop2)");
            } else {
                runtest.fail("as_value(Element &->prop2)");
            }
        }
    }

    std::auto_ptr<Element> el1 = as1.to_element();
    Element *fooel = el1->getProperty(0);
    Element *barel = el1->getProperty(1);
    if ((el1->getType() == Element::OBJECT_AMF0)
        && (fooel->getType() == Element::STRING_AMF0)
        && (strcmp(fooel->getName(), "foo") == 0)
        && (strcmp(fooel->to_string(), "bar") == 0)
        && (barel->getType() == Element::NUMBER_AMF0)
        && (strcmp(barel->getName(), "bar") == 0)
        && (barel->to_number() == 1.234)
        ) {
        runtest.pass("as_value::to_element()");
    } else {
        runtest.fail("as_value::to_element()");
    }
    
}

void
test_isnan()
{
	float num = 0;

//	std::cout << "sizeof(as_value): " << (sizeof(as_value)) << std::endl;

	if(!isNaN(num)) {
            runtest.pass("isNaN(0)");
        } else {
            runtest.fail("isNaN(0)");
        }
        
	num /= 9999999;

	if(!isNaN(num)) {
            runtest.pass("isNaN(9999999)");
        } else {
            runtest.fail("isNaN(9999999)");
        }
	if(utility::isFinite(num)) {
            runtest.pass("isFinite(9999999)");
        } else {
            runtest.fail("isFinite(9999999)");
        }

	num = std::numeric_limits<float>::quiet_NaN();

	if(isNaN(num)) {
            runtest.pass("isNaN(quiet_NaN)");
        } else {
            runtest.fail("isNaN(quiet_NaN)");
        }
	if(!utility::isFinite(num)) {
            runtest.pass("isFinite(quiet_NaN)");
        } else {
            runtest.fail("isFinite(quiet_NaN)");
        }

	num = std::numeric_limits<float>::infinity();
	
	if(!isNaN(num)) {
            runtest.pass("isNaN(infinity)");
        } else {
            runtest.fail("isNaN(infinity)");
        }
	if(!utility::isFinite(num)) {
            runtest.pass("isFinite(infinity)");
        } else {
            runtest.fail("isFinite(infinity)");
        }

	num = 1.0 / 0.0;

	if(!isNaN(num)) {
            runtest.pass("isNaN(1.0/0.0)");
        } else {
            runtest.fail("isNaN(1.0/0.0)");
        }
	if(!utility::isFinite(num)) {
            runtest.pass("isFinite(1.0/0.0)");
        } else {
            runtest.fail("isFinite(1.0/0.0)");
        }

	int intgr = num;

	num = intgr;

	if(!isNaN(num)) {
            runtest.pass("isNaN(int)");
        } else {
            runtest.fail("isNaN(int)");
        }
	if(utility::isFinite(num)) {
            runtest.pass("isFinite(int)");
        } else {
            runtest.fail("isFinite(int)");
        }
}

static void
usage (void)
{
    cerr << "This program tests the as_value class." << endl
         << endl
         << _("Usage: AsValue [options...]") << endl
         << _("  -h,  --help          Print this help and exit") << endl
         << _("  -v,  --verbose       Output verbose debug info") << endl
         << endl;
}

#else

int
main(int /*argc*/, char /* *argv[]*/)
{
  // nop
  return 0;  
}

#endif

