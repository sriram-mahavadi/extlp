/* 
 * File:   Fraction.h
 * Author: sriram
 *
 * Created on 13 March, 2014, 5:30 PM
 */

#ifndef FRACTION_H
#define	FRACTION_H
class Fraction{
private:
    int numerator;
    int denominator;
public:
    Fraction(){
        numerator=0, denominator=1;
    }
    Fraction(int numerator){
        this->numerator = numerator;
    }
    Fraction(int numerator, int denominator){
        this->numerator = numerator;
        this->denominator = denominator;
    }
    int getDenominator(){
        return this->denominator;
    }
    int getNumerator(){
        return this->numerator;
    }
    Fraction& operator+(Fraction fraction){
        int lcmDenom = getLcm(denominator, fraction->getDenominator());
        int lcmNum = (lcmDenom/denominator)*numerator + 
                        (lcmDenom/fraction->getDenominator())*fraction->getNumerator();
        int hcf = getHcf(lcmDenom, lcmNum);
        Fraction *newFraction = new Fraction(lcmNum/hcf, lcmDenom/hcf);
        return *newFraction;
    }
};


#endif	/* FRACTION_H */

