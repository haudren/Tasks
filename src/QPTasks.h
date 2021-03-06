// This file is part of Tasks.
//
// Tasks is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tasks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Tasks.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

// includes
// Eigen
#include <Eigen/Core>

// Tasks
#include "Tasks.h"
#include "QPSolver.h"

// forward declaration
// RBDyn
namespace rbd
{
class MultiBody;
class MultiBodyConfig;
}

namespace tasks
{

namespace qp
{


class SetPointTaskCommon : public Task
{
public:
	SetPointTaskCommon(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double weight);
	SetPointTaskCommon(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, const Eigen::VectorXd& dimWeight,
		double weight);

	virtual std::pair<int, int> begin() const
	{
		return std::make_pair(alphaDBegin_, alphaDBegin_);
	}

	void dimWeight(const Eigen::VectorXd& dim);

	const Eigen::VectorXd& dimWeight() const
	{
		return dimWeight_;
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

protected:
	void computeQC(Eigen::VectorXd& error);

protected:
	HighLevelTask* hlTask_;
	Eigen::VectorXd error_;

private:
	Eigen::VectorXd dimWeight_;
	int robotIndex_, alphaDBegin_;

	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
	// cache
	Eigen::MatrixXd preQ_;
	Eigen::VectorXd preC_;
};


class SetPointTask : public SetPointTaskCommon
{
public:
	SetPointTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double stiffness, double weight);

	SetPointTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double stiffness, const Eigen::VectorXd& dimWeight,
		double weight);

	double stiffness() const
	{
		return stiffness_;
	}

	void stiffness(double stiffness);

	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

private:
	double stiffness_, stiffnessSqrt_;
};


class TrackingTask : public SetPointTaskCommon
{
public:
	TrackingTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double gainPos, double gainVel, double weight);

	TrackingTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double gainPos, double gainVel,
		const Eigen::VectorXd& dimWeight, double weight);

	void setGains(double gainPos, double gainVel);

	void errorPos(const Eigen::VectorXd& errorPos);
	void errorVel(const Eigen::VectorXd& errorVel);
	void refAccel(const Eigen::VectorXd& refAccel);

	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

private:
	double gainPos_, gainVel_;
	Eigen::VectorXd errorPos_, errorVel_, refAccel_;
};


class TrajectoryTask : public SetPointTaskCommon
{
public:
	TrajectoryTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double gainPos, double gainVel, double weight);

	TrajectoryTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double gainPos, double gainVel,
		const Eigen::VectorXd& dimWeight, double weight);

	void setGains(double gainPos, double gainVel);

	void refVel(const Eigen::VectorXd& refVel);
	void refAccel(const Eigen::VectorXd& refAccel);

	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

private:
	double gainPos_, gainVel_;
	Eigen::VectorXd refVel_, refAccel_;
};


/// @deprecated Must be replace by TrackingTask
class PIDTask : public SetPointTaskCommon
{
public:
	PIDTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double P, double I, double D, double weight);

	PIDTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask, double P, double I, double D,
		const Eigen::VectorXd& dimWeight, double weight);

	double P() const;
	void P(double p);
	double I() const;
	void I(double i);
	double D() const;
	void D(double d);

	void error(const Eigen::VectorXd& err);
	void errorD(const Eigen::VectorXd& errD);
	void errorI(const Eigen::VectorXd& errI);

	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

private:
	double P_, I_, D_;
	Eigen::VectorXd error_, errorD_, errorI_;
};


class TargetObjectiveTask : public Task
{
public:
	TargetObjectiveTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask,
		double timeStep, double duration, const Eigen::VectorXd& objDot,
		double weight);

	TargetObjectiveTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hlTask,
		double timeStep, double duration, const Eigen::VectorXd& objDot,
		const Eigen::VectorXd& dimWeight, double weight);

	double duration() const;
	void duration(double d);

	int iter() const
	{
		return iter_;
	}
	void iter(int i)
	{
		iter_ = i;
	}

	int nrIter() const
	{
		return nrIter_;
	}
	void nrIter(int i)
	{
		nrIter_ = i;
	}

	const Eigen::VectorXd& objDot() const
	{
		return objDot_;
	}
	void objDot(const Eigen::VectorXd& o)
	{
		objDot_ = o;
	}

	const Eigen::VectorXd& dimWeight() const
	{
		return dimWeight_;
	}
	void dimWeight(const Eigen::VectorXd& o)
	{
		dimWeight_ = o;
	}

	const Eigen::VectorXd& phi() const
	{
		return phi_;
	}
	const Eigen::VectorXd& psi() const
	{
		return psi_;
	}


	virtual std::pair<int, int> begin() const
	{
		return std::make_pair(alphaDBegin_, alphaDBegin_);
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

private:
	HighLevelTask* hlTask_;

	int iter_, nrIter_;
	double dt_;
	Eigen::VectorXd objDot_;
	Eigen::VectorXd dimWeight_;
	int robotIndex_, alphaDBegin_;

	Eigen::VectorXd phi_, psi_;

	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
	// cache
	Eigen::MatrixXd preQ_;
	Eigen::VectorXd CVecSum_, preC_;
};



class JointsSelector : public HighLevelTask
{
public:
	static JointsSelector ActiveJoints(const std::vector<rbd::MultiBody>& mbs,
		int robotIndex, HighLevelTask* hl, const std::vector<int>& activeJointsId);
	static JointsSelector UnactiveJoints(const std::vector<rbd::MultiBody>& mbs,
		int robotIndex, HighLevelTask* hl, const std::vector<int>& unactiveJointsId);

public:
	struct SelectedData
	{
		int posInDof, dof;
	};

public:
	JointsSelector(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		HighLevelTask* hl, const std::vector<int>& selectedJointsId);

	const std::vector<SelectedData> selectedJoints() const
	{
		return selectedJoints_;
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	Eigen::MatrixXd jac_;
	std::vector<SelectedData> selectedJoints_;
	HighLevelTask* hl_;
};



struct JointStiffness
{
	JointStiffness():
		jointId(),
		stiffness()
	{}
	JointStiffness(int jId, double stif):
		jointId(jId),
		stiffness(stif)
	{}

	int jointId;
	double stiffness;
};

struct JointGains
{
	JointGains():
		jointId(),
		stiffness(),
		damping()
	{}
	JointGains(int jId, double stif) :
		jointId(jId),
		stiffness(stif)
	{
		damping = 2.*std::sqrt(stif);
	}

	JointGains(int jId, double stif, double damp):
		jointId(jId),
		stiffness(stif),
		damping(damp)
	{}

	int jointId;
	double stiffness, damping;
};


class PostureTask : public Task
{
public:
	PostureTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		std::vector<std::vector<double> > q, double stiffness, double weight);

	tasks::PostureTask& task()
	{
		return pt_;
	}

	void posture(std::vector<std::vector<double> > q)
	{
		pt_.posture(q);
	}

	const std::vector<std::vector<double> > posture() const
	{
		return pt_.posture();
	}

	double stiffness() const
	{
		return stiffness_;
	}

	double damping() const
	{
		return damping_;
	}

	void stiffness(double stiffness);

	void gains(double stiffness);
	void gains(double stiffness, double damping);

	void jointsStiffness(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<JointStiffness>& jsv);

	void jointsGains(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<JointGains>& jgv);

	virtual std::pair<int, int> begin() const
	{
		return std::make_pair(alphaDBegin_, alphaDBegin_);
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

	const Eigen::VectorXd& eval() const;

private:
	struct JointData
	{
		double stiffness, damping;
		int start, size;
	};

private:
	tasks::PostureTask pt_;

	double stiffness_;
	double damping_;
	int robotIndex_, alphaDBegin_;

	std::vector<JointData> jointDatas_;

	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
	Eigen::VectorXd alphaVec_;
};



class PositionTask : public HighLevelTask
{
public:
	PositionTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const Eigen::Vector3d& pos,
		const Eigen::Vector3d& bodyPoint=Eigen::Vector3d::Zero());

	tasks::PositionTask& task()
	{
		return pt_;
	}

	void position(const Eigen::Vector3d& pos)
	{
		pt_.position(pos);
	}

	const Eigen::Vector3d& position() const
	{
		return pt_.position();
	}

	void bodyPoint(const Eigen::Vector3d& point)
	{
		pt_.bodyPoint(point);
	}

	const Eigen::Vector3d& bodyPoint() const
	{
		return pt_.bodyPoint();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mb,
		const std::vector<rbd::MultiBodyConfig>& mbc,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::PositionTask pt_;
	int robotIndex_;
};



class OrientationTask : public HighLevelTask
{
public:
	OrientationTask(const std::vector<rbd::MultiBody>& mbs, int robodIndex,
		int bodyId, const Eigen::Quaterniond& ori);
	OrientationTask(const std::vector<rbd::MultiBody>& mbs, int robodIndex,
		int bodyId, const Eigen::Matrix3d& ori);

	tasks::OrientationTask& task()
	{
		return ot_;
	}

	void orientation(const Eigen::Quaterniond& ori)
	{
		ot_.orientation(ori);
	}

	void orientation(const Eigen::Matrix3d& ori)
	{
		ot_.orientation(ori);
	}

	const Eigen::Matrix3d& orientation() const
	{
		return ot_.orientation();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::OrientationTask ot_;
	int robotIndex_;
};



template <typename transform_task_t>
class TransformTaskCommon : public HighLevelTask
{
public:
	TransformTaskCommon(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const sva::PTransformd& X_0_t,
		const sva::PTransformd& X_b_p=sva::PTransformd::Identity());

	tasks::TransformTask& task()
	{
		return tt_;
	}

	void target(const sva::PTransformd& X_0_t)
	{
		tt_.target(X_0_t);
	}

	const sva::PTransformd& target() const
	{
		return tt_.target();
	}

	void X_b_p(const sva::PTransformd& X_b_p)
	{
		tt_.X_b_p(X_b_p);
	}

	const sva::PTransformd& X_b_p() const
	{
		return tt_.X_b_p();
	}

	virtual int dim();

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

protected:
	transform_task_t tt_;
	int robotIndex_;
};



/// TransformTask in surface frame.
class SurfaceTransformTask : public TransformTaskCommon<tasks::SurfaceTransformTask>
{
public:
	SurfaceTransformTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const sva::PTransformd& X_0_t,
		const sva::PTransformd& X_b_p=sva::PTransformd::Identity());

	virtual void update(const std::vector<rbd::MultiBody>& mb,
		const std::vector<rbd::MultiBodyConfig>& mbc,
		const SolverData& data);
};



/// TransformTask in world or user frame.
class TransformTask : public TransformTaskCommon<tasks::TransformTask>
{
public:
	TransformTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const sva::PTransformd& X_0_t,
		const sva::PTransformd& X_b_p=sva::PTransformd::Identity(),
		const Eigen::Matrix3d& E_0_c=Eigen::Matrix3d::Identity());

	void E_0_c(const Eigen::Matrix3d& E_0_c);
	const Eigen::Matrix3d& E_0_c() const;

	virtual void update(const std::vector<rbd::MultiBody>& mb,
		const std::vector<rbd::MultiBodyConfig>& mbc,
		const SolverData& data);
};



class SurfaceOrientationTask : public HighLevelTask
{
public:
	SurfaceOrientationTask(const std::vector<rbd::MultiBody>& mbs, int robodIndex,
			int bodyId, const Eigen::Quaterniond& ori, const sva::PTransformd& X_b_s);
	SurfaceOrientationTask(const std::vector<rbd::MultiBody>& mbs, int robodIndex,
			int bodyId, const Eigen::Matrix3d& ori, const sva::PTransformd& X_b_s);

	tasks::SurfaceOrientationTask& task()
	{
			return ot_;
	}

	void orientation(const Eigen::Quaterniond& ori)
	{
			ot_.orientation(ori);
	}

	void orientation(const Eigen::Matrix3d& ori)
	{
			ot_.orientation(ori);
	}

	const Eigen::Matrix3d& orientation() const
	{
			return ot_.orientation();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
			const std::vector<rbd::MultiBodyConfig>& mbcs,
			const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::SurfaceOrientationTask ot_;
	int robotIndex_;
};



class GazeTask : public HighLevelTask
{
public:
	GazeTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const Eigen::Vector2d& point2d, double depthEstimate,
		const sva::PTransformd& X_b_gaze,
		const Eigen::Vector2d& point2d_ref = Eigen::Vector2d::Zero());
	GazeTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const Eigen::Vector3d& point3d, const sva::PTransformd& X_b_gaze,
		const Eigen::Vector2d& point2d_ref = Eigen::Vector2d::Zero());

	tasks::GazeTask& task()
	{
			return gazet_;
	}

	void error(const Eigen::Vector2d& point2d,
		const Eigen::Vector2d& point2d_ref = Eigen::Vector2d::Zero())
	{
			gazet_.error(point2d, point2d_ref);
	}

	void error(const Eigen::Vector3d& point3d,
		const Eigen::Vector2d& point2d_ref = Eigen::Vector2d::Zero())
	{
			gazet_.error(point3d, point2d_ref);
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
			const std::vector<rbd::MultiBodyConfig>& mbcs, const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::GazeTask gazet_;
	int robotIndex_;
};



class CoMTask : public HighLevelTask
{
public:
	CoMTask(const std::vector<rbd::MultiBody>& mbs,
		int robotIndex, const Eigen::Vector3d& com);
	CoMTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		const Eigen::Vector3d& com, std::vector<double> weight);

	tasks::CoMTask& task()
	{
		return ct_;
	}

	void com(const Eigen::Vector3d& com)
	{
		ct_.com(com);
	}

	const Eigen::Vector3d com() const
	{
		return ct_.com();
	}

	void updateInertialParameters(const std::vector<rbd::MultiBody>& mbs);

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::CoMTask ct_;
	int robotIndex_;
};


class MultiCoMTask : public Task
{
public:
	MultiCoMTask(const std::vector<rbd::MultiBody>& mb,
		std::vector<int> robotIndexes, const Eigen::Vector3d& com,
		double stiffness, double weight);
	MultiCoMTask(const std::vector<rbd::MultiBody>& mb,
		std::vector<int> robotIndexes, const Eigen::Vector3d& com,
		double stiffness, const Eigen::Vector3d& dimWeight, double weight);

	tasks::MultiCoMTask& task()
	{
		return mct_;
	}

	void com(const Eigen::Vector3d& com)
	{
		mct_.com(com);
	}

	const Eigen::Vector3d com() const
	{
		return mct_.com();
	}

	void updateInertialParameters(const std::vector<rbd::MultiBody>& mbs);

	double stiffness() const
	{
		return stiffness_;
	}

	void stiffness(double stiffness);

	void dimWeight(const Eigen::Vector3d& dim);

	const Eigen::Vector3d& dimWeight() const
	{
		return dimWeight_;
	}

	virtual std::pair<int, int> begin() const
	{
		return {alphaDBegin_, alphaDBegin_};
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

	const Eigen::VectorXd& eval() const;
	const Eigen::VectorXd& speed() const;

private:
	void init(const std::vector<rbd::MultiBody>& mbs);

private:
	int alphaDBegin_;
	double stiffness_, stiffnessSqrt_;
	Eigen::Vector3d dimWeight_;
	std::vector<int> posInQ_;
	tasks::MultiCoMTask mct_;
	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
	Eigen::Vector3d CSum_;
	// cache
	Eigen::MatrixXd preQ_;
};


class MultiRobotTransformTask : public Task
{
public:
	MultiRobotTransformTask(const std::vector<rbd::MultiBody>& mbs,
		int r1Index, int r2Index, int r1BodyId, int r2BodyId,
		const sva::PTransformd& X_r1b_r1s, const sva::PTransformd& X_r2b_r2s,
		double stiffness, double weight);

	tasks::MultiRobotTransformTask& task()
	{
		return mrtt_;
	}

	void X_r1b_r1s(const sva::PTransformd& X_r1b_r1s);
	const sva::PTransformd& X_r1b_r1s() const;

	void X_r2b_r2s(const sva::PTransformd& X_r2b_r2s);
	const sva::PTransformd& X_r2b_r2s() const;

	double stiffness() const
	{
		return stiffness_;
	}

	void stiffness(double stiffness);

	void dimWeight(const Eigen::Vector6d& dim);

	const Eigen::Vector6d& dimWeight() const
	{
		return dimWeight_;
	}

	virtual std::pair<int, int> begin() const
	{
		return {alphaDBegin_, alphaDBegin_};
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

	const Eigen::VectorXd& eval() const;
	const Eigen::VectorXd& speed() const;

private:
	int alphaDBegin_;
	double stiffness_, stiffnessSqrt_;
	Eigen::Vector6d dimWeight_;
	std::vector<int> posInQ_, robotIndexes_;
	tasks::MultiRobotTransformTask mrtt_;
	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
	Eigen::Vector6d CSum_;
	// cache
	Eigen::MatrixXd preQ_;
};


class MomentumTask : public HighLevelTask
{
public:
	MomentumTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		const sva::ForceVecd& mom);

	tasks::MomentumTask& task()
	{
		return momt_;
	}

	void momentum(const sva::ForceVecd& mom)
	{
		momt_.momentum(mom);
	}

	const sva::ForceVecd momentum()
	{
		return momt_.momentum();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mb,
		const std::vector<rbd::MultiBodyConfig>& mbc,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::MomentumTask momt_;
	int robotIndex_;
};


class ContactTask : public Task
{
public:
	ContactTask(ContactId contactId, double stiffness, double weight):
		Task(weight),
		contactId_(contactId),
		begin_(0),
		stiffness_(stiffness),
		stiffnessSqrt_(2*std::sqrt(stiffness)),
		conesJac_(),
		error_(Eigen::Vector3d::Zero()),
		errorD_(Eigen::Vector3d::Zero()),
		Q_(),
		C_()
	{}

	virtual std::pair<int, int> begin() const
	{
		return std::make_pair(begin_, begin_);
	}

	void error(const Eigen::Vector3d& error);
	void errorD(const Eigen::Vector3d& errorD);

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

private:
	ContactId contactId_;
	int begin_;

	double stiffness_, stiffnessSqrt_;
	Eigen::MatrixXd conesJac_;
	Eigen::Vector3d error_, errorD_;

	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
};



class GripperTorqueTask : public Task
{
public:
	GripperTorqueTask(ContactId contactId, const Eigen::Vector3d& origin,
		const Eigen::Vector3d& axis, double weight):
		Task(weight),
		contactId_(contactId),
		origin_(origin),
		axis_(axis),
		begin_(0),
		Q_(),
		C_()
	{}

	virtual std::pair<int, int> begin() const
	{
		return std::make_pair(begin_, begin_);
	}

	virtual void updateNrVars(const std::vector<rbd::MultiBody>& mbs,
		const SolverData& data);
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& Q() const;
	virtual const Eigen::VectorXd& C() const;

private:
	ContactId contactId_;
	Eigen::Vector3d origin_;
	Eigen::Vector3d axis_;
	int begin_;

	Eigen::MatrixXd Q_;
	Eigen::VectorXd C_;
};



class LinVelocityTask : public HighLevelTask
{
public:
	LinVelocityTask(const std::vector<rbd::MultiBody>& mbs, int robotIndex,
		int bodyId, const Eigen::Vector3d& vel,
		const Eigen::Vector3d& bodyPoint=Eigen::Vector3d::Zero());

	tasks::LinVelocityTask& task()
	{
		return pt_;
	}

	void velocity(const Eigen::Vector3d& s)
	{
		pt_.velocity(s);
	}

	const Eigen::Vector3d& velocity() const
	{
		return pt_.velocity();
	}

	void bodyPoint(const Eigen::Vector3d& point)
	{
		pt_.bodyPoint(point);
	}

	const Eigen::Vector3d& bodyPoint() const
	{
		return pt_.bodyPoint();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	tasks::LinVelocityTask pt_;
	int robotIndex_;
};



class OrientationTrackingTask : public HighLevelTask
{
public:
	OrientationTrackingTask(const std::vector<rbd::MultiBody>& mbs,
		int robotIndex, int bodyId,
		const Eigen::Vector3d& bodyPoint, const Eigen::Vector3d& bodyAxis,
		const std::vector<int>& trackingJointsId,
		const Eigen::Vector3d& trackedPoint);

	tasks::OrientationTrackingTask& task()
	{
		return ott_;
	}

	void trackedPoint(const Eigen::Vector3d& tp)
	{
		ott_.trackedPoint(tp);
	}

	const Eigen::Vector3d& trackedPoint() const
	{
		return ott_.trackedPoint();
	}

	void bodyPoint(const Eigen::Vector3d& bp)
	{
		ott_.bodyPoint(bp);
	}

	const Eigen::Vector3d& bodyPoint() const
	{
		return ott_.bodyPoint();
	}

	void bodyAxis(const Eigen::Vector3d& ba)
	{
		ott_.bodyAxis(ba);
	}

	const Eigen::Vector3d& bodyAxis() const
	{
		return ott_.bodyAxis();
	}

	virtual int dim();
	virtual void update(const std::vector<rbd::MultiBody>& mbs,
		const std::vector<rbd::MultiBodyConfig>& mbcs,
		const SolverData& data);

	virtual const Eigen::MatrixXd& jac();
	virtual const Eigen::VectorXd& eval();
	virtual const Eigen::VectorXd& speed();
	virtual const Eigen::VectorXd& normalAcc();

private:
	int robotIndex_;
	tasks::OrientationTrackingTask ott_;
	Eigen::VectorXd alphaVec_;
	Eigen::VectorXd speed_, normalAcc_;
};

} // namespace qp

} // namespace tasks
