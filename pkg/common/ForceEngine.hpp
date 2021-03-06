// 2004 © Janek Kozicki <cosurgi@berlios.de> 
// 2009 © Václav Šmilauer <eudoxos@arcig.cz> 
// 2014 © Raphael Maurin <raphael.maurin@irstea.fr> 

#pragma once

#include<core/PartialEngine.hpp>

class ForceEngine: public PartialEngine{
	public:
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS(ForceEngine,PartialEngine,"Apply contact force on some particles at each step.",
		((Vector3r,force,Vector3r::Zero(),,"Force to apply."))
	);
};
REGISTER_SERIALIZABLE(ForceEngine);

/* Engine for applying force of varying magnitude but constant direction
 * on subscribed bodies. times and magnitudes must have the same length,
 * direction (normalized automatically) gives the orientation.
 *
 * As usual with interpolating engines: the first magnitude is used before the first
 * time point, last magnitude is used after the last time point. Wrap specifies whether
 * time wraps around the last time point to the first time point.
 */
class InterpolatingDirectedForceEngine: public ForceEngine{
	size_t _pos;
	public:
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS_CTOR(InterpolatingDirectedForceEngine,ForceEngine,"Engine for applying force of varying magnitude but constant direction on subscribed bodies. times and magnitudes must have the same length, direction (normalized automatically) gives the orientation. \n\n\
	\
	As usual with interpolating engines: the first magnitude is used before the first time point, last magnitude is used after the last time point. Wrap specifies whether time wraps around the last time point to the first time point.",
		((vector<Real>,times,,,"Time readings [s]"))
		((vector<Real>,magnitudes,,,"Force magnitudes readings [N]"))
		((Vector3r,direction,Vector3r::UnitX(),,"Contact force direction (normalized automatically)"))
		((bool,wrap,false,,"wrap to the beginning of the sequence if beyond the last time point")),
		/*ctor*/ _pos=0
	);
};
REGISTER_SERIALIZABLE(InterpolatingDirectedForceEngine);

struct RadialForceEngine: public PartialEngine{
	virtual void action();
	virtual void postLoad(RadialForceEngine&);
	YADE_CLASS_BASE_DOC_ATTRS(RadialForceEngine,PartialEngine,"Apply force of given magnitude directed away from spatial axis.",
		((Vector3r,axisPt,Vector3r::Zero(),,"Point on axis"))
		((Vector3r,axisDir,Vector3r::UnitX(),Attr::triggerPostLoad,"Axis direction (normalized automatically)"))
		((Real,fNorm,0,,"Applied force magnitude"))
	);
};
REGISTER_SERIALIZABLE(RadialForceEngine);

class DragEngine: public PartialEngine{
	public:
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS(DragEngine,PartialEngine,"Apply `drag force <http://en.wikipedia.org/wiki/Drag_equation>`__ on some particles at each step, decelerating them proportionally to their linear velocities. The applied force reads\n\n.. math:: F_{d}=-\\frac{\\vec{v}}{|\\vec{v}|}\\frac{1}{2}\\rho|\\vec{v}|^2 C_d A\n\nwhere $\\rho$ is the medium density (:yref:`density<DragEngine.Rho>`), $v$ is particle's velocity,  $A$ is particle projected area (disc), $C_d$ is the drag coefficient (0.47 for :yref:`Sphere`), \n\n.. note:: Drag force is only applied to spherical particles, listed in ids.",
		((Real,Rho,1.225,,"Density of the medium (fluid or air), by default - the density of the air."))
		((Real,Cd,0.47,,"Drag coefficient <http://en.wikipedia.org/wiki/Drag_coefficient>`_."))
	);
};
REGISTER_SERIALIZABLE(DragEngine);

class LinearDragEngine: public PartialEngine{
	public:
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS(LinearDragEngine,PartialEngine,"Apply `viscous resistance or linear drag <http://en.wikipedia.org/wiki/Drag_%28physics%29#Very_low_Reynolds_numbers_.E2.80.94_Stokes.27_drag>`__ on some particles at each step, decelerating them proportionally to their linear velocities. The applied force reads\n\n.. math:: F_{d}=-b{\\vec{v}} \n\nwhere $b$ is the linear drag, $\\vec{v}$ is particle's velocity. \n\n.. math:: b=6\\pi\\nu r \n\nwhere $\\nu$ is the medium viscosity, $r$ is the `Stokes radius <http://en.wikipedia.org/wiki/Stokes_radius>`__ of the particle (but in this case we accept it equal to sphere radius for simplification), \n\n.. note:: linear drag is only applied to spherical particles, listed in ids.",
		((Real,nu,0.001,,"Viscosity of the medium."))
	);
};
REGISTER_SERIALIZABLE(LinearDragEngine);


class HydroForceEngine: public PartialEngine{
	public:
		void averageProfile();
		void turbulentFluctuation();
		void turbulentFluctuationBIS();
		void turbulentFluctuationFluidizedBed();
	public:
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS_CTOR_PY(HydroForceEngine,PartialEngine,"Apply drag and lift due to a fluid flow vector (1D) to each sphere + the buoyant weight.\n The applied drag force reads\n\n $F_{d}=\\frac{1}{2} C_d A\\rho^f|\\vec{v_f - v}| \\vec{v_f - v}$ \n\n where $\\rho$ is the medium density (:yref:`densFluid<HydroForceEngine.densFluid>`), $v$ is particle's velocity,  $v_f$ is the velocity of the fluid at the particle center(:yref:`vxFluid<HydroForceEngine.vxFluid>`),  $A$ is particle projected area (disc), $C_d$ is the drag coefficient. The formulation of the drag coefficient depends on the local particle reynolds number and the solid volume fraction. The formulation of the drag is [Dallavalle1948]_ [RevilBaudard2013]_ with a correction of Richardson-Zaki [Richardson1954]_ to take into account the hindrance effect. This law is classical in sediment transport. It is possible to activate a fluctuation of the drag force for each particle which account for the turbulent fluctuation of the fluid velocity (:yref:`velFluct<HydroForceEngine.velFluct>`). The model implemented for the turbulent velocity fluctuation is a simple discrete random walk which takes as input the Reynolds stress tensor $R^f_{xz}$ as a function of the depth, and allows to recover the main property of the fluctuations by imposing $<u_x'u_z'> (z) = <R^f_{xz}>(z)/\\rho^f$. It requires as input $<R^f_{xz}>(z)/\\rho^f$ called :yref:`simplifiedReynoldStresses<HydroForceEngine.simplifiedReynoldStresses>` in the code. \n The formulation of the lift is taken from [Wiberg1985]_ and is such that : \n\n $F_{L}=\\frac{1}{2} C_L A\\rho^f((v_f - v)^2_{top} - (v_f - v)^2_{bottom})$ \n\n Where the subscript top and bottom means evaluated at the top (respectively the bottom) of the sphere considered. This formulation of the lift account for the difference of pressure at the top and the bottom of the particle inside a turbulent shear flow. As this formulation is controversial when approaching the threshold of motion [Schmeeckle2007]_ it is possible to desactivate it with the variable :yref:`lift<HydroForceEngine.lift>`.\n The buoyancy is taken into account through the buoyant weight : \n\n $F_{buoyancy}= - \\rho^f V^p g$ \n\n, where g is the gravity vector along the vertical, and $V^p$ is the volume of the particle. This engine also evaluate the average particle velocity, solid volume fraction and drag force depth profiles, through the function averageProfile. This is done as the solid volume fraction depth profile is required for the drag calculation, and as the three are required for the independent fluid resolution.",
		((Real,densFluid,1000,,"Density of the fluid, by default - density of water"))
		((Real,viscoDyn,1e-3,,"Dynamic viscosity of the fluid, by default - viscosity of water"))
		((Real,zRef,,,"Position of the reference point which correspond to the first value of the fluid velocity, i.e. to the ground."))
		((Real,deltaZ,,,"Height of the discretization cell."))
		((Real,expoRZ,3.1,,"Value of the Richardson-Zaki exponent, for the drag correction due to hindrance"))
                ((bool,lift,false,,"Option to activate or not the evaluation of the lift"))
		((Real,Cl,0.2,,"Value of the lift coefficient taken from [Wiberg1985]_"))
		((Real,vCell,,,"Volume of averaging cell"))
		((int,nCell,,,"Number of cell in the depth"))
                ((Vector3r,gravity,Vector3r(0,0,-9.81),,"Gravity vector (may depend on the slope)."))
		((vector<Real>,vxFluid,,,"Discretized streamwise fluid velocity depth profile"))
		((vector<Real>,phiPart,,,"Discretized solid volume fraction depth profile. Can be taken as input parameter, or evaluated directly inside the engine, calling from python the averageProfile() function, or puting :yref:`activateAverage<HydroForceEngine.activateAverage>` to True."))
		((vector<Real>,vxPart,,,"Discretized streamwise solid velocity depth profile. Can be taken as input parameter, or evaluated directly inside the engine, calling from python the averageProfile() function, or puting :yref:`activateAverage<HydroForceEngine.activateAverage>` to True."))
                ((vector<Real>,vyPart,,,"Discretized spanwise solid velocity depth profile. No role in the engine, output parameter. For practical reason, it can be evaluated directly inside the engine, calling from python the averageProfile() method of the engine, or puting :yref:`activateAverage<HydroForceEngine.activateAverage>` to True."))
                ((vector<Real>,vzPart,,,"Discretized normal solid velocity depth profile. No role in the engine, output parameter. For practical reason, it can be evaluated directly inside the engine, calling from python the averageProfile() method of the engine, or puting :yref:`activateAverage<HydroForceEngine.activateAverage>` to True."))
		((vector<Real>,averageDrag,,,"Discretized average drag depth profile. No role in the engine, output parameter. For practical reason, it can be evaluated directly inside the engine, calling from python the averageProfile() method of the engine, or puting :yref:`activateAverage<HydroForceEngine.activateAverage>` to True."))
                ((bool,twoSize,false,,"Option to activate when considering two particle size in the simulation. When activated evaluate the average solid volume fraction and drag force for the two type of particles of diameter diameterPart1 and diameterPart2 independently."))
		((Real,radiusPart1,0.,,"Radius of the particles of type 1. Useful only when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((Real,radiusPart2,0.,,"Radius of the particles of type 2. Useful only when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,phiPart1,,,"Discretized solid volume fraction depth profile of particles of type 1. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,phiPart2,,,"Discretized solid volume fraction depth profile of particles of type 2. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,averageDrag1,,,"Discretized average drag depth profile of particles of type 1. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,averageDrag2,,,"Discretized average drag depth profile of particles of type 2. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vxPart1,,,"Discretized solid streamwise velocity depth profile of particles of type 1. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vxPart2,,,"Discretized solid streamwise velocity depth profile of particles of type 2. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vyPart1,,,"Discretized solid spanwise velocity depth profile of particles of type 1. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vyPart2,,,"Discretized solid spanwise velocity depth profile of particles of type 2. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vzPart1,,,"Discretized solid wall-normal velocity depth profile of particles of type 1. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((vector<Real>,vzPart2,,,"Discretized solid wall-normal velocity depth profile of particles of type 2. Evaluated when :yref:`twoSize<HydroForceEngine.twoSize>` is set to True."))
		((bool,activateAverage,false,,"If true, activate the calculation of the average depth profiles of drag, solid volume fraction, and solid velocity for the application of the force (phiPart in hindrance function) and to use in python for the coupling with the fluid."))
		((bool,velFluct,false,,"If true, activate the determination of turbulent fluid velocity fluctuation for the next time step only at the position of each particle, using a simple discrete random walk (DRW) model based on the Reynolds stresses profile (:yref:`simplifiedReynoldStresses<HydroForceEngine.simplifiedReynoldStresses>`)"))
		((vector<Real>,vFluctX,,,"Vector associating a streamwise fluid velocity fluctuation to each particle. Fluctuation calculated in the C++ code from the discrete random walk model"))
		((vector<Real>,vFluctY,,,"Vector associating a spanwise fluid velocity fluctuation to each particle. Fluctuation calculated in the C++ code from the discrete random walk model"))
		((vector<Real>,vFluctZ,,,"Vector associating a normal fluid velocity fluctuation to each particle. Fluctuation calculated in the C++ code from the discrete random walk model"))
		((vector<Real>,simplifiedReynoldStresses,,,"Vector of size equal to :yref:`nCell<HydroForceEngine.nCell>` containing the Reynolds stresses divided by the fluid density in function of the depth. simplifiedReynoldStresses(z) $=  <u_x'u_z'>(z)^2$"))
		((Real,bedElevation,,,"Elevation of the bed above which the fluid flow is turbulent and the particles undergo turbulent velocity fluctuation."))
		((vector<Real>,fluctTime,,,"Vector containing the time of life of the fluctuations associated to each particles."))
		((Real,dtFluct,,,"Execution time step of the turbulent fluctuation model."))

	,/*ctor*/
	,/*py*/
	 .def("averageProfile",&HydroForceEngine::averageProfile,"Compute and store the particle velocity (:yref:`vxPart<HydroForceEngine.vxPart>`, :yref:`vyPart<HydroForceEngine.vyPart>`, :yref:`vzPart<HydroForceEngine.vzPart>`) and solid volume fraction (:yref:`phiPart<HydroForceEngine.phiPart>`) depth profile. For each defined cell z, the k component of the average particle velocity reads: \n\n $<v_k>^z= \\sum_p V^p v_k^p/\\sum_p V^p$,\n\n where the sum is made over the particles contained in the cell, $v_k^p$ is the k component of the velocity associated to particle p, and $V^p$ is the part of the volume of the particle p contained inside the cell. This definition allows to smooth the averaging, and is equivalent to taking into account the center of the particles only when there is a lot of particles in each cell. As for the solid volume fraction, it is evaluated in the same way:  for each defined cell z, it reads: \n\n $<\\phi>^z= \\frac{1}{V_{cell}}\\sum_p V^p$, where $V_{cell}$ is the volume of the cell considered, and $V^p$ is the volume of particle p contained in cell z.\n This function gives depth profiles of average velocity and solid volume fraction, returning the average quantities in each cell of height dz, from the reference horizontal plane at elevation :yref:`zRef<HydroForceEngine.zRef>` (input parameter) until the plane of elevation :yref:`zRef<HydroForceEngine.zRef>` plus :yref:`nCell<HydroForceEngine.nCell>` times :yref:`deltaZ<HydroForceEngine.deltaZ>` (input parameters). When the option :yref:`twoSize<HydroForceEngine.twoSize>` is set to True, evaluate in addition the average drag (:yref:`averageDrag1<HydroForceEngine.averageDrag1>` and :yref:`averageDrag2<HydroForceEngine.averageDrag2>`) and solid volume fraction (:yref:`phiPart1<HydroForceEngine.phiPart1>` and :yref:`phiPart2<HydroForceEngine.phiPart2>`) depth profiles considering only the particles of radius respectively :yref:`radiusPart1<HydroForceEngine.radiusPart1>` and :yref:`radiusPart2<HydroForceEngine.radiusPart2>` in the averaging.")
	 .def("turbulentFluctuation",&HydroForceEngine::turbulentFluctuation,"Apply turbulent fluctuation to the problem.")
	 .def("turbulentFluctuationBIS",&HydroForceEngine::turbulentFluctuationBIS,"Apply turbulent fluctuation to the problem with an alternative formulation.")
	 .def("turbulentFluctuationFluidizedBed",&HydroForceEngine::turbulentFluctuationFluidizedBed,"Apply turbulent fluctuation to the problem with another alternative formulation.")
	);
};
REGISTER_SERIALIZABLE(HydroForceEngine);

