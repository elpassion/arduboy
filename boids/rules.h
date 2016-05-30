#ifndef RULES_H_
#define RULES_H_

#include "boid.h"
#include "environment.h"
#include "point.h"

class Rule {
  public:
    explicit Rule(Boid** boids, const byte boids_count) : boids_(boids), boids_count_(boids_count) {}
    virtual Point Compute(const Boid& boid, const Environment& environment) const = 0;

  protected:
    Boid** boids_;
    byte   boids_count_;
};

class CenterOfMassRule : public Rule {
  public:
    using Rule::Rule;
    Point Compute(const Boid& boid, const Environment& environment) const;
};

class KeepDistanceRule : public Rule {
  public:
    using Rule::Rule;
    Point Compute(const Boid& boid, const Environment& environment) const;
};

class MatchVelocityRule : public Rule {
  public:
    using Rule::Rule;
    Point Compute(const Boid& boid, const Environment& environment) const;
};

class KeepWithinBoundsRule : public Rule {
  public:
    KeepWithinBoundsRule(Boid** boids, const byte boids_count, Point bounds) : Rule(boids, boids_count), bounds_(bounds) {}
    Point Compute(const Boid& boid, const Environment& environment) const;
    
  private:
    Point bounds_;
};

#endif
