#include "XmlBackground2DFile.h"

#include "Background2D.h"
#include "Background2DAnimation.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "Logger.h"




XmlBackground2DFile::XmlBackground2DFile( const Ogre::String& file ):
    XmlFile( file )
{
}



XmlBackground2DFile::~XmlBackground2DFile()
{
}



void
XmlBackground2DFile::Load()
{
    TiXmlNode* node = m_File.RootElement();
    Background2D* background = EntityManager::getSingleton().GetBackground2D();

    if( node == NULL || node->ValueStr() != "background2d" )
    {
        LOG_ERROR( "XML 2D Background loader: " + m_File.ValueStr() + " is not a valid background2d file! No <background2d> in root." );
        return;
    }

    Ogre::String image = GetString( node, "image", "" );

    if( image != "" )
    {
        background->SetImage( image );

        Ogre::Vector3 position = GetVector3( node, "position", Ogre::Vector3::ZERO );
        Ogre::Quaternion orientation = GetQuaternion( node, "orientation", Ogre::Quaternion::IDENTITY );
        float fov = GetFloat( node, "fov", 90 );

        CameraManager::getSingleton().GetCurrentCamera()->setPosition( position );
        CameraManager::getSingleton().GetCurrentCamera()->setOrientation( orientation );
        CameraManager::getSingleton().GetCurrentCamera()->setFOVy( Ogre::Radian( Ogre::Degree( fov ) ) );

        int tile_id = 0;

        node = node->FirstChild();
        while( node != NULL )
        {
            if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "tile" )
            {
                int width = GetInt( node, "width", 0 );
                int height = GetInt( node, "height", 0 );

                Ogre::Vector2 destination = GetVector2( node, "destination", Ogre::Vector2::ZERO );
                Ogre::Vector4 uv = GetVector4( node, "uv", Ogre::Vector4::ZERO );
                float depth = GetFloat( node, "depth", 0 );

                background->AddTile( destination.x, destination.y, width, height, depth, uv.x, uv.y, uv.z, uv.w );

                TiXmlNode* node2 = node->FirstChild();
                while( node2 != NULL )
                {
                    if( node2->Type() == TiXmlNode::TINYXML_ELEMENT && node2->ValueStr() == "animation" )
                    {
                        Ogre::String name = GetString( node2, "name", "" );
                        if( name != "" )
                        {
                            Background2DAnimation* animation = new Background2DAnimation( name, background, tile_id );

                            animation->SetLength( GetFloat( node2, "length", 0 ) );

                            Ogre::String uv = GetString( node2, "uv", "" );
                            if( uv != "" )
                            {
                                Ogre::StringVector key_frame = Ogre::StringUtil::split( uv, "," );
                                for( unsigned int i = 0; i < key_frame.size(); ++i )
                                {
                                    Ogre::StringUtil::trim( key_frame[ i ] );

                                    Ogre::StringVector data = Ogre::StringUtil::split( key_frame[ i ], ":" );
                                    if( data.size() > 1 )
                                    {
                                        float time = Ogre::StringConverter::parseReal( data[ 0 ] );
                                        Ogre::Vector4 value = Ogre::StringConverter::parseVector4( data[ 1 ] );
                                        animation->AddUVKeyFrame( time, value.x, value.y, value.z, value.w );
                                    }
                                }
                            }

                            background->AddAnimation( animation );
                        }
                    }

                    node2 = node2->NextSibling();
                }

                ++tile_id;
            }

            node = node->NextSibling();
        }
    }
}
