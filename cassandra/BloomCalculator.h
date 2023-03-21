#ifndef BLOOMCALCULATOR_H
#define BLOOMCALCULATOR_H

using namespace std;

class BloomSpecification {
    public:
        int K;
        int bucketsPerElement;
        BloomSpecification(int K, int bucketsPerElement) {
            this->K = K;
            this->bucketsPerElement = bucketsPerElement;
        }
};

class BloomCalculator
{
    private:
        static const int minBuckets = 2;
        static const int minK = 1;
        static vector<vector<double>> probs;
        static vector<int> optKPerBuckets;
        
    public:
        static shared_ptr<BloomSpecification> computeBloomSpec(int maxBucketsPerElement, double maxFalsePosProb);
        static shared_ptr<BloomSpecification> computeBloomSpec(int bucketsPerElement);
        static int maxBucketsPerElement(long numElements);
        
};

#endif