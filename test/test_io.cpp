//  (C) Copyright Andy Tompkins 2009. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_io.cpp  -------------------------------//

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace boost::uuids;

template<class _S, class _T>
void fail(const _T& in)
{
    uuid out;
    _S ss;
    ss << in;
    ss >> out;
    BOOST_TEST_EQ(true, ss.fail());
}

void roundtrip(const uuid& in, const std::string& expect)
{
    std::stringstream ss;
    ss << in;
    BOOST_TEST_EQ(false, ss.fail());
    BOOST_TEST_EQ(expect, ss.str());

    uuid out;
    std::stringstream ss2;
    ss2 << expect;
    ss2 >> out;
    BOOST_TEST_EQ(false, ss2.fail());
    BOOST_TEST_EQ(in, out);
}

#ifndef BOOST_NO_STD_WSTRING
void roundtrip(const uuid& in, const std::wstring& expect)
{
    std::wstringstream ss;
    ss << in;
    BOOST_TEST_EQ(false, ss.fail());
    BOOST_TEST   (expect == ss.str());

    uuid out;
    std::wstringstream ss2;
    ss2 << expect;
    ss2 >> out;
    BOOST_TEST_EQ(false, ss2.fail());
    BOOST_TEST   (in == out);
}
#endif

int main(int, char*[])
{
    const uuid u1 = {{0}};
    const uuid u2 = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
    const uuid u3 = {{0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef,0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef}};

    roundtrip(u1, "00000000-0000-0000-0000-000000000000");
    roundtrip(u2, "00010203-0405-0607-0809-0a0b0c0d0e0f");
    roundtrip(u3, "12345678-90ab-cdef-1234-567890abcdef");

    std::stringstream ss5;
    ss5 << 'a' << std::right << std::setfill('*') << std::setw(40) << u1 << 'a';
    BOOST_TEST_EQ(ss5.str(), "a****00000000-0000-0000-0000-000000000000a");

    std::stringstream ss6;
    ss6 << std::left << std::setfill('*') << std::setw(45) << u1;
    BOOST_TEST_EQ(ss6.str(), "00000000-0000-0000-0000-000000000000*********");

    std::stringstream ss7;
    ss7 << std::left << std::setfill('*') << std::setw(45) << u2;
    BOOST_TEST_EQ(ss7.str(), "00010203-0405-0607-0809-0a0b0c0d0e0f*********");

#ifndef BOOST_NO_STD_WSTRING
    roundtrip(u1, L"00000000-0000-0000-0000-000000000000");
    roundtrip(u2, L"00010203-0405-0607-0809-0a0b0c0d0e0f");
    roundtrip(u3, L"12345678-90ab-cdef-1234-567890abcdef");
#endif

    fail<std::stringstream>(std::string("01234567-89ab-cdef-\0123-456789abcdef"));
    fail<std::stringstream>(std::string("01234567-89ab-cdef-0123456789abcdef"));
    fail<std::stringstream>(std::string("01234567-89ab-cdef0123-456789abcdef"));
    fail<std::stringstream>(std::string("01234567-89abcdef-0123-456789abcdef"));
    fail<std::stringstream>(std::string("0123456789ab-cdef-0123-456789abcdef"));
    fail<std::stringstream>(std::string("{12345678-90ab-cdef-1234-567890abcdef}"));  // stream is not as flexible as string
    fail<std::stringstream>(std::string("G1234567-89AB-CDEF-0123-456789abcdef"));
    fail<std::stringstream>(std::string("01234567-89AB-CDEF-0123-456789abcdeg"));

#ifndef BOOST_NO_STD_WSTRING
    fail<std::wstringstream>(std::wstring(L"01234567-89ab-cdef-\0123-456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"01234567-89ab-cdef-0123456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"01234567-89ab-cdef0123-456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"01234567-89abcdef-0123-456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"0123456789ab-cdef-0123-456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"{12345678-90ab-cdef-1234-567890abcdef}"));  // stream is not as flexible as string
    fail<std::wstringstream>(std::wstring(L"G1234567-89AB-CDEF-0123-456789abcdef"));
    fail<std::wstringstream>(std::wstring(L"01234567-89AB-CDEF-0123-456789abcdeg"));
#endif

    BOOST_TEST(boost::lexical_cast<std::string>(u1) == std::string("00000000-0000-0000-0000-000000000000"));
    BOOST_TEST(boost::lexical_cast<uuid>("00000000-0000-0000-0000-000000000000") == u1);

    BOOST_TEST(boost::lexical_cast<std::string>(u3) == std::string("12345678-90ab-cdef-1234-567890abcdef"));
    BOOST_TEST(boost::lexical_cast<uuid>("12345678-90ab-cdef-1234-567890abcdef") == u3);

#ifndef BOOST_NO_STD_WSTRING
    BOOST_TEST(boost::lexical_cast<std::wstring>(u1) == std::wstring(L"00000000-0000-0000-0000-000000000000"));
    BOOST_TEST(boost::lexical_cast<uuid>(L"00000000-0000-0000-0000-000000000000") == u1);

    BOOST_TEST(boost::lexical_cast<std::wstring>(u3) == std::wstring(L"12345678-90ab-cdef-1234-567890abcdef"));
    BOOST_TEST(boost::lexical_cast<uuid>(L"12345678-90ab-cdef-1234-567890abcdef") == u3);
#endif

    BOOST_TEST(to_string(u1) == std::string("00000000-0000-0000-0000-000000000000"));
    BOOST_TEST(to_string(u3) == std::string("12345678-90ab-cdef-1234-567890abcdef"));

#ifndef BOOST_NO_STD_WSTRING
    BOOST_TEST(to_wstring(u1) == std::wstring(L"00000000-0000-0000-0000-000000000000"));
    BOOST_TEST(to_wstring(u3) == std::wstring(L"12345678-90ab-cdef-1234-567890abcdef"));
#endif

    return boost::report_errors();
}
