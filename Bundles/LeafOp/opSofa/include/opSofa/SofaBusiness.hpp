#ifndef OPSOFA_SOFABUSINESS_HPP_
#define OPSOFA_SOFABUSINESS_HPP_

#include <sofa/component/typedef/Sofa_typedef.h>
#include <sofa/component/visualmodel/OglModel.h>
#include <sofa/simulation/tree/treeSimulation.h>
#include <sofa/simulation/tree/GNode.h>

#include <fwServices/IEditionService.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/PatientDB.hpp>

#include "opSofa/SofaThread.hpp"


using namespace sofa::simulation::tree;
using sofa::component::visualmodel::OglModel;
using namespace sofa::defaulttype;

class SofaThread;

/**
 * @brief Manages the SOFA framework
 */
class SofaBusiness
{

public:
    SofaBusiness();
    virtual ~SofaBusiness();
    void loadScn(std::string, ::fwData::Acquisition::sptr, ::fwServices::IService::sptr);
    void loadMesh(::fwData::Mesh::sptr, ::fwServices::IService::sptr);
    unsigned int getTimeStepAnimation();
    void setTimeStepAnimation(unsigned int timeStepAnimation);
    void animate();
    void startThread();
    void stopThread();
    bool isAnimate();
    void reset();
    void shakeMesh(std::string, int);
    void moveMesh(std::string, int, int, int, float, float, float);

private:
    void fillOglModelVector(GNode*, std::vector<OglModel*>*);
    void fillSpringForceField(GNode*, std::map<std::string, StiffSpringForceField3*>*);
    void fillMeshVector(::fwData::Acquisition::sptr, std::vector<fwData::Mesh::sptr>*);
    void translationPointer(OglModel*, ::fwData::Mesh::sptr);
    void clearTranslationPointer();

    /**
     * @brief Pointer to the GNode object containing the physical parameters
     */
    GNode::SPtr groot;

    /**
     * @brief Data containing the step of deformation
     */
    unsigned int timeStepAnimation;

    /**
     * @brief Pointer to the SofaThread object
     */
    SofaThread *thread;

    /**
     * @brief Vector to mesh object
     */
    std::vector< ::fwData::Mesh::sptr > *meshs;

    /**
     * @brief map to StiffSpringForceField3 object
     */
    std::map<std::string, StiffSpringForceField3*> *springs;

};

#endif // OPSOFA_SOFABUSINESS_HPP_
