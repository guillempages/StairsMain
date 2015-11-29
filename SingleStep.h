#ifndef _SINGLE_STEP_H_
#define _SINGLE_STEP_H_

class SingleStep {
public:

    enum StepValues {
        LedValue = 0,
        IrThreshold,
        IrValue,
        LightThreshold,
        LightValue,
        Dummy,
        Count
    };

    static uint8_t id;

    SingleStep(uint8_t index=12) { for (uint8_t i=0; i<Count; i++) values[i] =  id*10 + i; id++;}

    uint8_t values[Count];
};

#endif // _SINGLE_STEP_H_
