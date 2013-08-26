/*
-----------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2013-08-26 Tobias Peters <tobias.peters@kreativeffekt.at>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#define BOOST_TEST_MODULE QGearsBackground2DFileSerializer
#include <boost/test/unit_test.hpp>

#include <Ogre.h>

#include "map/QGearsBackground2DFileXMLSerializer.h"

BOOST_AUTO_TEST_CASE( read_file )
{
    class TestFile : public QGears::Background2DFile
    {
    public:
        TestFile() : QGears::Background2DFile( NULL, "", 0, "" ) {}
        size_t getCalculatedSize() const { return calculateSize(); }

        virtual void unload( void ) {};
    };

	Ogre::LogManager                    logMgr;
    logMgr.createLog( "Default Log", true, true, true );

    const char*                             file_name( "reference.background2d.xml" );
    TestFile                                file;
    QGears::Background2DFileXMLSerializer   ser;
    std::ifstream *ifs(  OGRE_NEW_T( std::ifstream, Ogre::MEMCATEGORY_GENERAL )( file_name, std::ifstream::binary ) );
    BOOST_REQUIRE( ifs->is_open() );

    Ogre::DataStreamPtr stream( OGRE_NEW Ogre::FileStreamDataStream( ifs ) );
    BOOST_REQUIRE( stream->isReadable() );

    ser.importBackground2DFile( stream, &file );
    BOOST_CHECK_EQUAL( 137956, stream->tell() );
    BOOST_CHECK_EQUAL( "maps/ffvii/field/md1stin.png", file.getTextureName() );

    logMgr.destroyLog( "Default Log" );
    ifs->close();
}