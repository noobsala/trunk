# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./plugins/Engine/ConstitutiveLaw/MassSpringLaw
# Target is a library:  

LIBS += -lSerialization \
        -lMesh2D \
        -lMath \
        -lBody \
        -lInteraction \
        -lMultiMethods \
        -lSpringPhysics \
        -lSpringGeometry \
        -lActionForce \
        -lActionMomentum \
        -lParticleParameters \
        -lEngine \
        -rdynamic 
INCLUDEPATH += $(YADEINCLUDEPATH) 
MOC_DIR = $(YADECOMPILATIONPATH) 
UI_DIR = $(YADECOMPILATIONPATH) 
OBJECTS_DIR = $(YADECOMPILATIONPATH) 
QMAKE_LIBDIR = ../../../../toolboxes/Libraries/Serialization/$(YADEDYNLIBPATH) \
               ../../../../plugins/Body/GeometricalModel/Mesh2D/$(YADEDYNLIBPATH) \
               ../../../../toolboxes/Libraries/Math/$(YADEDYNLIBPATH) \
               ../../../../yade/Body/Body/$(YADEDYNLIBPATH) \
               ../../../../yade/Interaction/Interaction/$(YADEDYNLIBPATH) \
               ../../../../toolboxes/Libraries/MultiMethods/$(YADEDYNLIBPATH) \
               ../../../../plugins/Interaction/InteractionPhysics/SpringPhysics/$(YADEDYNLIBPATH) \
               ../../../../plugins/Interaction/InteractionGeometry/SpringGeometry/$(YADEDYNLIBPATH) \
               ../../../../plugins/Engine/Action/ActionForce/$(YADEDYNLIBPATH) \
               ../../../../plugins/Engine/Action/ActionMomentum/$(YADEDYNLIBPATH) \
               ../../../../plugins/Body/BodyPhysicalParameters/ParticleParameters/$(YADEDYNLIBPATH) \
               $(YADEDYNLIBPATH) 
QMAKE_CXXFLAGS_RELEASE += -lpthread \
                          -pthread 
QMAKE_CXXFLAGS_DEBUG += -lpthread \
                        -pthread 
DESTDIR = $(YADEDYNLIBPATH) 
CONFIG += debug \
          warn_on \
          dll 
TEMPLATE = lib 
HEADERS += MassSpringLaw.hpp 
SOURCES += MassSpringLaw.cpp 
