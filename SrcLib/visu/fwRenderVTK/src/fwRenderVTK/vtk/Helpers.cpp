/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkPicker.h>
#include <vtkAssemblyNode.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkAssemblyPath.h>
#include <vtkProp3DCollection.h>
#include <vtkShader2Collection.h>
#include <vtkShaderProgram2.h>
#include <vtkShader2.h>
#include <vtkOpenGLRenderWindow.h>

#include <fwCore/base.hpp>

#include "fwRenderVTK/vtk/Helpers.hpp"

namespace fwRenderVTK
{

namespace vtk
{

//------------------------------------------------------------------------------

vtkIdType getNearestPointId(vtkPoints* pts, vtkRenderer* renderer)
{
    vtkIdType id=-1;
    double camPosition[3];
    double distance = VTK_DOUBLE_MAX;
    renderer->GetActiveCamera()->GetPosition(camPosition);

    for(vtkIdType i = 0; i<pts->GetNumberOfPoints (); i++)
    {
        double *point = pts->GetPoint(i);
        double distancePtCam = vtkMath::Distance2BetweenPoints(point, camPosition);

        if(distancePtCam < distance)
        {
            id = i;
            distance = distancePtCam;
        }
    }

    return id;
}

//------------------------------------------------------------------------------

vtkProp * getNearestPickedProp(vtkAbstractPropPicker *picker, vtkRenderer *renderer)
{
    vtkProp   *res       = NULL;
    vtkPicker *vtkpicker = vtkPicker::SafeDownCast(picker);

    SLM_ASSERT("getNearestPickedProp *need* a picker.", picker);
    SLM_ASSERT("getNearestPickedProp *need* a renderer.", renderer);

    if (vtkpicker)
    {
        vtkIdType id = getNearestPointId(vtkpicker->GetPickedPositions(), renderer);

        if (id>-1 && vtkpicker->GetProp3Ds()->GetNumberOfItems() > id)
        {
//          vtkIdType i = id;
//          vtkAssemblyPath *path = picker->GetPath();
//          vtkAssemblyNode *node;
//          path->InitTraversal();
//          for ( path->InitTraversal(); (node=path->GetNextNode()); )
//          {
//              if (--i == 0)
//              {
//                  res = node->GetViewProp();
//                  break;
//              }
//          }
//          vtkAssemblyNode *node = vtkAssemblyNode::SafeDownCast(picker->GetPath()->GetItemAsObject(id));
//          SLM_ASSERT("vtkAssemblyNode error: not found", node);
//          res = node->GetViewProp();
//          SLM_ASSERT("No vtkProp found in picker's path", res);

            res = vtkProp::SafeDownCast(vtkpicker->GetProp3Ds()->GetItemAsObject(id));
        }
    }
    else
    {
        res = picker->GetProp3D();
    }
    return res;
}

//------------------------------------------------------------------------------

bool getNearestPickedPosition(vtkAbstractPropPicker *picker, vtkRenderer *renderer, double position[3])
{
    bool res = false;
    vtkPicker *vtkpicker = vtkPicker::SafeDownCast(picker);

    SLM_ASSERT("getNearestPickedProp *need* a picker.", picker);
    SLM_ASSERT("getNearestPickedProp *need* a renderer.", renderer);

    double *point = NULL;
    if (vtkpicker)
    {
        vtkPoints *pts = vtkpicker->GetPickedPositions();
        vtkIdType id = getNearestPointId(pts, renderer);

        if (id>-1)
        {
            point = pts->GetPoint(id);
        }
        else
        {
            point = vtkpicker->GetPickPosition();
        }
    }
    else
    {
        point = picker->GetPickPosition();
    }

    if(point)
    {
        std::copy(point, point + 3, position);
        res = true;
    }
    return res;
}

//------------------------------------------------------------------------------

vtkSmartPointer<vtkShaderProgram2> buildShader( vtkOpenGLRenderWindow* pWindow,
                                                const char* pcVertexShader,
                                                const char* pcFragmentShader )
{
    SLM_ASSERT( "NULL parameter", pWindow && pcVertexShader && pcFragmentShader );

    vtkOpenGLRenderWindow *pOpenGLWindow = vtkOpenGLRenderWindow::SafeDownCast(pWindow);
    if(!pOpenGLWindow)
    {
        SLM_ERROR("Shader only supported using OpenGL.");
        return NULL;
    }

    // Check if GLSL is supported
    if (!vtkShaderProgram2::IsSupported(pOpenGLWindow))
    {
        SLM_ERROR("GLSL is not supported on this system.");
        return NULL;
    }

    vtkSmartPointer<vtkShaderProgram2> pProgram = vtkSmartPointer<vtkShaderProgram2>::New();
    pProgram->SetContext(pOpenGLWindow);

    {
        // The vertex shader
        vtkShader2 *shader = vtkShader2::New();
        shader->SetType(VTK_SHADER_TYPE_VERTEX);
        shader->SetSourceCode(pcVertexShader);
        shader->SetContext(pProgram->GetContext());
        pProgram->GetShaders()->AddItem(shader);
        shader->Delete();
    }
    {
        // The fragment shader
        vtkShader2 *shader = vtkShader2::New();
        shader->SetType(VTK_SHADER_TYPE_FRAGMENT);
        shader->SetSourceCode(pcFragmentShader);
        shader->SetContext(pProgram->GetContext());
        pProgram->GetShaders()->AddItem(shader);
        shader->Delete();
    }

    return pProgram;
}

//------------------------------------------------------------------------------

void openShader(const char* _pcName, std::string& _strShader)
{
    // Read data.txt
    std::string line;
    std::ifstream myfile(_pcName);
    if ( myfile.is_open() )
    {
        while ( myfile.good() )
        {
            getline( myfile, line );
            _strShader += line + "\n";
        }
        myfile.close();
    }
    else
    {
        SLM_ERROR("Unable to open file : " + std::string(_pcName));
    }
}

//------------------------------------------------------------------------------

vtkSmartPointer<vtkShaderProgram2> buildShaderFromFile( vtkRenderWindow* pWindow,
                                                        const char* pcVertexName,
                                                        const char* pcFragmentName )
{
    SLM_ASSERT( "NULL parameter", pWindow && pcVertexName && pcFragmentName );

    vtkOpenGLRenderWindow *pOpenGLWindow = vtkOpenGLRenderWindow::SafeDownCast(pWindow);
    if(!pOpenGLWindow)
    {
        SLM_ERROR("Shader only supported using OpenGL.");
        return NULL;
    }

    // Check if GLSL is supported
    if (!vtkShaderProgram2::IsSupported(pOpenGLWindow))
    {
        SLM_ERROR("GLSL is not supported on this system.");
        return NULL;
    }

    vtkSmartPointer<vtkShaderProgram2> pProgram = vtkSmartPointer<vtkShaderProgram2>::New();
    pProgram->SetContext(pOpenGLWindow);

    {
        // The vertex shader
        std::string strShader("");
        openShader(pcVertexName, strShader);

        vtkShader2 *shader = vtkShader2::New();
        shader->SetType(VTK_SHADER_TYPE_VERTEX);
        shader->SetSourceCode(strShader.c_str());
        shader->SetContext(pProgram->GetContext());
        pProgram->GetShaders()->AddItem(shader);
        shader->Delete();
    }
    {
        // The fragment shader
        std::string strShader("");
        openShader(pcFragmentName, strShader);

        vtkShader2 *shader = vtkShader2::New();
        shader->SetType(VTK_SHADER_TYPE_FRAGMENT);
        shader->SetSourceCode(strShader.c_str());
        shader->SetContext(pProgram->GetContext());
        pProgram->GetShaders()->AddItem(shader);
        shader->Delete();
    }

    return pProgram;
}

} //vtk

} //fwRenderVTK



