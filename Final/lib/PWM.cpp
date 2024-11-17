
#include "PWM.hpp"

PWM::PWM(PinPWM pinPWM, uint8_t pourcentage){
    pourcentage_ = pourcentage;
    pin_ = pinPWM;
    switch(pinPWM) {
        case PinPWM::B04:
            TCNT0 = 0;
            OCR0A = (pourcentage_ * 255) / 100;
            TCCR0A |= (1 << COM0A1 | 1 << WGM00);
            TCCR0B |= (1 << CS00);
            DDRB |= (1 << PB3);
            break;
        case PinPWM::B05:
            TCNT0 = 0;
            OCR0B = (pourcentage_ * 255) / 100;
            TCCR0A |= (1 << COM0B1 | 1 << WGM00);
            TCCR0B |= (1 << CS00);
            DDRB |= (1 << PB4);
            break;
        case PinPWM::D15:
            TCNT1 = 0;
            OCR1B = (pourcentage_ * 255) / 100;
            TCCR1A |= (1 << COM1B1 | 1 << WGM10);
            TCCR1B |= (1 << CS10);
            DDRD |= (1 << PD4);
            break;
        case PinPWM::D16:
            TCNT1 = 0;
            OCR1A = (pourcentage_ * 255) / 100;
            TCCR1A |= (1 << COM1A1 | 1 << WGM10);
            TCCR1B |= (1 << CS10);
            DDRD |= (1 << PD5);
            break;
        case PinPWM::D27:
            TCNT2 = 0;
            OCR2B = (pourcentage_ * 255) / 100;
            TCCR2A |= (1 << COM2B1 | 1 << WGM20);
            TCCR2B |= (1 << CS20);
            DDRD |= (1 << PD6);
            break;
        case PinPWM::D28:
            TCNT2 = 0;
            OCR2A = (pourcentage_ * 255) / 100;
            TCCR2A |= (1 << COM2A1 | 1 << WGM20);
            TCCR2B |= (1 << CS20);
            DDRD |= (1 << PD7);
            break;
        default:
            break;
    }
};

void PWM::setPourcentage(uint8_t pourcentage){
    pourcentage_ = pourcentage;
    if (pourcentage_ > 100){
        pourcentage_ = 100;
    }
    switch(pin_) {
        case PinPWM::B04:
            OCR0A = (pourcentage_ * 255) / 100;
            break;
        case PinPWM::B05:
            OCR0B = (pourcentage_ * 255) / 100;
            break;
        case PinPWM::D15:
            OCR1B = (pourcentage_ * 255) / 100;
            break;
        case PinPWM::D16:
            OCR1A = (pourcentage_ * 255) / 100;
            break;
        case PinPWM::D27:
            OCR2B = (pourcentage_ * 255) / 100;
            break;
        case PinPWM::D28:
            OCR2A = (pourcentage_ * 255) / 100;
            break;
        default:
            break;
    }

};

void PWM::augmenterPourcentage(uint8_t pourcentage){
    uint8_t difference = 100 - pourcentage;
    if (difference <= pourcentage_){
        pourcentage_ = 100;
    }
    else{
        pourcentage_ += pourcentage;
    }
    setPourcentage(pourcentage_);
};

void PWM::diminuerPourcentage(uint8_t pourcentage){
    if (pourcentage > pourcentage_){
        pourcentage_ = 0;
        }
    else{
        pourcentage_ -= pourcentage;
    }
    setPourcentage(pourcentage_);
};

