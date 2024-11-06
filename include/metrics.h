#ifndef METRICS_H
#define METRICS_H

#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <unordered_map>

class Metrics {
public:
    struct MetricsData {
        int classCount = 0;
        int methodCount = 0;
        int ifCount = 0;
        int forCount = 0;
        int whileCount = 0;
        int switchCount = 0;
        int caseCount = 0;
        int variableCount = 0;
        int commentCount = 0;
    };

    std::vector<std::string> files;

    Metrics(const std::vector<std::string>& files);

    const MetricsData& getMetrics() const;

    void printMetrics() const;

private:
    static void collectMetrics(CXCursor cursor, MetricsData& data);

    void aggregateMetrics(const MetricsData& fileMetrics);

    void printMetricsData(const MetricsData& data) const;

    MetricsData metrics;  // Aggregated metrics
    std::unordered_map<std::string, MetricsData> fileMetricsMap;  // Per-file metrics
};

#endif //AST_BUILDER_METRICS_H
