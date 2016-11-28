#ifndef DIS2OBST_CONSTRAINT_H
#define DIS2OBST_CONSTRAINT_H

#include <path_follower/local_planner/constraint.h>

class Dis2Obst_Constraint : public Constraint
{
public:
    typedef std::shared_ptr<Dis2Obst_Constraint> Ptr;

public:
    Dis2Obst_Constraint();
    virtual ~Dis2Obst_Constraint();

    virtual bool isSatisfied(const LNode& point) override;
};

#endif // DIS2OBST_CONSTRAINT_H