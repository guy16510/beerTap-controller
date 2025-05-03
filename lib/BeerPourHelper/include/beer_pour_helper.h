// src/beer_pour_helper.h
#ifndef BEER_POUR_HELPER_H
#define BEER_POUR_HELPER_H

#include <stdint.h>
#include "constants.h"  // for BEER_SECONDS_PER_PINT

/**
 * BeerPourHelper tracks the volume of beer currently being poured in ounces.
 * It resets to zero when pouring stops.
 */
class BeerPourHelper {
public:
    /**
     * Initialize helper; call once in setup().
     */
    static void begin();

    /**
     * Update with current pouring state; call each loop iteration.
     * @param pouring true if beer is currently pouring
     */
    static void update(bool pouring);

    /**
     * Get the current poured volume in fluid ounces since the last reset.
     * @return poured volume in ounces
     */
    static float currentOunces();

private:
    // Timestamp of last update in microseconds
    static uint64_t _lastMicros;
    // Accumulator for poured volume in ounces
    static float    _accumOunces;
    // Flow rate: ounces per second, derived from BEER_SECONDS_PER_PINT
    static constexpr float FLOW_OZ_PER_SEC = 16.0f / BEER_SECONDS_PER_PINT;
};

#endif // BEER_POUR_HELPER_H