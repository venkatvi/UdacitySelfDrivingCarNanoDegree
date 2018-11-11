/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

static default_random_engine gen;

/* init function creates a normal distribtuon of particles given (x,y, theta) as mean
* and std[] as std deviation.
*/
void ParticleFilter::init(double x, double y, double theta, double std[]) {
    num_particles = 50;
    normal_distribution<double> dist_x(x, std[0]);
    normal_distribution<double> dist_y(y, std[1]);
    normal_distribution<double> dist_theta(theta, std[2]);
    for (int i = 0; i < num_particles; ++i) {
      struct Particle p;
      p.id = i;
      p.x = dist_x(gen);
      p.y = dist_y(gen);
      p.theta = dist_theta(gen);
      p.weight = 1;
      weights.push_back(1);
      particles.push_back(p);
    }
    is_initialized = 1;
    return;
}

/*
* prediction function predicts the [x,y, theta] location of all particles given current timestep (delta_t), velocity and yaw_rate
* from the motion model
*/
void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
    normal_distribution<double> dist_x(0, std_pos[0]);
	normal_distribution<double> dist_y(0, std_pos[1]);
	normal_distribution<double> dist_theta(0, std_pos[2]);
    for (int i = 0; i < num_particles; ++i) {
      // The equation is modified for small yaw rates
      // 0.0001 was selected after some experiments
      if(fabs(yaw_rate) < 0.0001) {  
	    particles[i].x = particles[i].x + velocity*delta_t*cos(particles[i].theta);
	    particles[i].y = particles[i].y + velocity*delta_t*sin(particles[i].theta);
      } 
	  else {
        particles[i].x = particles[i].x + velocity/yaw_rate * (sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta)) + dist_x(gen);
        particles[i].y = particles[i].y + velocity/yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t)) + dist_y(gen);
        particles[i].theta = particles[i].theta + yaw_rate*delta_t + dist_theta(gen);
      }
    }
    return; 
}
/*
* This function associates the observations with the given predicted landmarks in order assign weights 
*/
void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
      for (unsigned i = 0; i < observations.size(); ++i) {
        double min_dist = 10000000000.0;
        int mapId;
        for (unsigned j = 0; j < predicted.size(); ++j) {
          double d = dist(observations[i].x, observations[i].y, predicted[j].x, predicted[j].y);
          if (d < min_dist) {
            min_dist = d;
            mapId = predicted[j].id;
          }
        }
        observations[i].id = mapId;
      }
  return;
}

/*
* This function updates weights given the sensor range data, the landmark information and map
*/
void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		const std::vector<LandmarkObs> &observations, const Map &map_landmarks) {
    for (unsigned i = 0; i < particles.size(); ++i) {
      double px = particles[i].x;
      double py = particles[i].y;
      double pt = particles[i].theta;
      std::vector<LandmarkObs> predicted;
      struct LandmarkObs temp_lm;
      for (unsigned j = 0; j < map_landmarks.landmark_list.size(); ++j) {
        double lmx = map_landmarks.landmark_list[j].x_f;
        double lmy = map_landmarks.landmark_list[j].y_f;
        int lmid = map_landmarks.landmark_list[j].id_i;
        double dist_p_lm = dist(px,py,lmx,lmy);
        temp_lm.x = lmx;
        temp_lm.y = lmy;
        temp_lm.id = lmid;
        if (dist_p_lm < sensor_range)
          predicted.push_back(temp_lm);
      }
      std::vector<LandmarkObs> new_observations;
      for (unsigned k = 0; k < observations.size(); ++k) {
        LandmarkObs obs;
        double ox = observations[k].x;
        double oy = observations[k].y;
        int oid = observations[k].id;
        obs.x = ox*cos(pt) - oy*sin(pt) + px;
        obs.y = ox*sin(pt) + oy*cos(pt) + py;
        obs.id = observations[k].id;
        new_observations.push_back(obs);
      }
      dataAssociation(predicted, new_observations);
      particles[i].weight = 1.0;
      for (int k = 0; k < new_observations.size(); ++k) {
        double ox = new_observations[k].x;
        double oy = new_observations[k].y;
        int oid = new_observations[k].id;
        double stdx = std_landmark[0];
        double stdy = std_landmark[1];
        double prx, pry;
        for (unsigned k = 0; k < predicted.size(); k++) {
          if (predicted[k].id == oid) {
            prx = predicted[k].x;
            pry = predicted[k].y;
          }
        }
        double prob_x_y = 1.0/(2*M_PI*stdx*stdy) * exp(-(pow((prx-ox),2)/2*pow(stdx,2)+pow((pry-oy),2)/2*pow(stdy,2)));
        particles[i].weight *= prob_x_y;
      }
    }
  return;
}

/* resample function resamples particles 
*/
void ParticleFilter::resample() {
  std::vector<double> weights;
  double maxW = -1000000000.0;
  for(int i = 0; i < num_particles; i++)
	weights.push_back(particles[i].weight);
  for(int i = 0; i < num_particles; i++) {
	if(particles[i].weight > maxW)
		maxW = particles[i].weight;
  }
  uniform_real_distribution<double> WeightDist(0.0, maxW);
  uniform_int_distribution<int> IndexDist(0, num_particles - 1);
  int index = IndexDist(gen);
  double beta = 0.0;
  std::vector<Particle> newParticles;
  for(int i = 0; i < num_particles; i++) {
	beta += WeightDist(gen) * 2.0;
	while(beta > weights[index]) {
		beta -= weights[index];
		index = (index + 1) % num_particles;
	}
	newParticles.push_back(particles[index]);
  }
  particles = newParticles;
  return;
}

Particle ParticleFilter::SetAssociations(Particle& particle, const std::vector<int>& associations, 
                                     const std::vector<double>& sense_x, const std::vector<double>& sense_y)
{
    //particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
    // associations: The landmark id that goes along with each listed association
    // sense_x: the associations x mapping already converted to world coordinates
    // sense_y: the associations y mapping already converted to world coordinates

    particle.associations= associations;
    particle.sense_x = sense_x;
    particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
