// $Id$

#include <Ogre.h>

#include "FieldCamera.h"



FieldCamera::FieldCamera(void):
    m_CameraState(CS_2_5D_GAME),
    m_CameraFreeRotate(false)
{
    // we must create this not in constructor of field cause this change camera.
    Ogre::SceneManager* scene_manager;
    scene_manager = Ogre::Root::getSingleton().getSceneManager("Scene");
    m_Camera = scene_manager->createCamera("FieldCamera");
    m_Camera->setNearClipDistance(5);
    m_Camera->setPosition(Ogre::Vector3(0, -1000, 1000));
    m_Camera->lookAt(Ogre::Vector3(0, 0, 0));

    Ogre::Viewport* viewport = Ogre::Root::getSingleton().getAutoCreatedWindow()->getViewport(0);
    m_PreviousCamera = viewport->getCamera();
    viewport->setCamera(m_Camera);
    m_Camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
}



FieldCamera::~FieldCamera(void)
{
    Ogre::SceneManager* scene_manager;
    scene_manager = Ogre::Root::getSingleton().getSceneManager("Scene");
    scene_manager->destroyCamera(scene_manager->getCamera("FieldCamera"));

    Ogre::Root::getSingleton().getAutoCreatedWindow()->getViewport(0)->setCamera(m_PreviousCamera);
}



void
FieldCamera::Input(const Event& input)
{
    if (input.name == "camera_change")
    {
        if (m_CameraState == CS_3D_FREE)
        {
            m_CameraState = CS_2_5D_GAME;

            m_Camera->setCustomViewMatrix(true, m_CameraViewMatrix);
            m_Camera->setCustomProjectionMatrix(true, m_CameraProjectionMatrix);
        }
        else
        {
            m_CameraState = CS_3D_FREE;

            m_Camera->setCustomViewMatrix(false, m_CameraViewMatrix);
            m_Camera->setCustomProjectionMatrix(false, m_CameraProjectionMatrix);
        }
    }



    if (m_CameraState == CS_3D_FREE)
    {
        //Ogre::LogManager::getSingletonPtr()->logMessage("Event come '" + input.name + "' and type '" + Ogre::StringConverter::toString(int(input.type)) + "'");
        if (input.name == "camera_forward")
        {
            m_Camera->moveRelative(Ogre::Vector3( 0, 0, -5));
        }
        if (input.name == "camera_left")
        {
            m_Camera->moveRelative(Ogre::Vector3(-5, 0,  0));
        }
        if (input.name == "camera_backward")
        {
            m_Camera->moveRelative(Ogre::Vector3( 0, 0,  5));
        }
        if (input.name == "camera_right")
        {
            m_Camera->moveRelative(Ogre::Vector3( 5, 0,  0));
        }

        if (input.name == "camera_rotate_on")
        {
            m_CameraFreeRotate = true;
        }
        else if (input.name == "camera_rotate_off")
        {
            m_CameraFreeRotate = false;
        }

        if (m_CameraFreeRotate && input.name == "mouse_move")
        {
            m_Camera->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(-input.x * 0.13)));
            m_Camera->pitch(Ogre::Degree(-input.y * 0.13));
        }
    }
}



void
FieldCamera::Update(const float delta_time)
{
}




void
FieldCamera::SetToPosition(const Ogre::Vector3& position)
{
    if (m_CameraState == CS_3D_FREE)
    {
        m_Camera->setPosition(position);
    }
}



void
FieldCamera::Set2_5DMatrixes(const Ogre::Matrix4& view, const Ogre::Matrix4& projection)
{
    m_CameraViewMatrix       = view;
    m_CameraProjectionMatrix = projection;

    if (m_CameraState == CS_2_5D_GAME)
    {
        m_Camera->setCustomViewMatrix(true, m_CameraViewMatrix);
        m_Camera->setCustomProjectionMatrix(true, m_CameraProjectionMatrix);
    }
}
