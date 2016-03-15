#ifndef PROBABILISTIC_FILTER_H
#define PROBABILISTIC_FILTER_H

#include <observation_interface.h>
#include <grid_structure.h>
#include <probablistic_cell.h>
#include <vector>
#include <utility>

#define KERNEL_MODEL 0
#define CONE_MODEL 1
#define LINE_MODEL 2
#define POSE_ARRAY 3
#define SENSOR_MODEL_TYPE 3

#define USE_IDEAL_LINE_SENSOR_MODEL 1

#define USE_RANGE_AND_NOISE_DECAY 1

#define USE_POSISITION_NOISE 0

#if SENSOR_MODEL_TYPE == KERNEL_MODEL
    #define RAY_END_BEFORE 1
#else
    #define RAY_END_BEFORE 0
#endif

class Probabilistic_filter : public Observation_interface
{
public:
    Probabilistic_filter(int xDim, int yDim, double resolution, double laserStdDev, double origin_x, double origin_y);
    ~Probabilistic_filter();

    // Traces a ray to x1,y1. All positions are written with a gaussian kernel based on lasernoise
    void raytrace(int x0, int y0, int x1, int y1, bool markEnd);

    // return the occupancy probability of the underlying cell
    double getOccupancyPrabability(int x, int y);

    // Loads the update bounds from grid structure
    void loadUpdateBounds(int& xMin, int& xMax, int& yMin, int& yMax);

    // Resets the grid structures update limits    
    void resetEditLimits();
    // raytrace with a sonar model
    void coneRayTrace(double ox, double oy, double tx, double ty, double angle_std_dev, bool mark_end);
    double _angle_std_dev;
    double _y_std_dev, _x_std_dev;
    double _raytrace_weight;
private:
    Grid_structure<Probablistic_cell>* _map;
    double _laser_noise_var;
    double _laser_noise_std_dev;

    bool enforceBounds(int& x, int& y);
    std::vector<double> _sensor_model;
    std::vector<double> _sensor_model_org;
#if USE_IDEAL_LINE_SENSOR_MODEL
    const static double _LOG_ODDS_FREE_ORG = -0.4055;
    double _LOG_ODDS_FREE;
#else
    const static double _LOG_ODDS_FREE_ORG = -1.09861228866811;
    double _LOG_ODDS_FREE;
#endif
    int _sensor_model_occupancy_goal_index;
    int _sensor_model_occupancy_goal_index_org;

    inline void bresenham2Dv0(int x1, int y1, int x2, int y2, bool markEnd = true);
    inline double getRangeWeight(int x1, int y1, int ori_x, int ori_y);
    // help to raytrace with a sonar model
    double delta(double phi);
    double sensor_model(double r, double phi, double theta);
    double gaussian_sensor_model(double r, double phi, double theta);
    double kernel_sensor_model(double r, double phi, double theta);
    double gamma(double theta);
    // half of field of view
    double _max_angle;
    double _phi_v;
    //static const double _sigma_r = 0.025;
    double _sigma_r;
};

#endif // PROBABILISTIC_FILTER_H
