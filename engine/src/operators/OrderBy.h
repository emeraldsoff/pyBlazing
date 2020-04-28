#ifndef BLAZINGDB_RAL_ORDERBY_OPERATOR_H
#define BLAZINGDB_RAL_ORDERBY_OPERATOR_H

#include <blazingdb/manager/Context.h>
#include <string>
#include <vector>
#include "execution_graph/logic_controllers/LogicPrimitives.h"


namespace ral {
namespace operators {
namespace experimental {

namespace {
  using blazingdb::manager::experimental::Context;
}  

std::unique_ptr<ral::frame::BlazingTable> process_sort(const ral::frame::BlazingTableView & table,
const std::string & query_part, Context * context);

/**---------------------------------------------------------------------------*
 * @brief Sorts the columns of the input table according the sortOrderTypes 
 * and sortColIndices.
 *
 * @param[in] table             table whose rows need to be compared for ordering
 * @param[in] sortColIndices    The vector of selected column indices to perform
 *                              the sort.
 * @param[in] sortOrderTypes    The expected sort order for each column. Size
 *                              must be equal to `sortColIndices.size()` or empty.
 *
 * @returns A BlazingTable with rows sorted.
 *---------------------------------------------------------------------------**/
std::unique_ptr<ral::frame::BlazingTable> logicalSort(
  const ral::frame::BlazingTableView & table, const std::vector<int> & sortColIndices, 
  const std::vector<int8_t> & sortOrderTypes);

/**---------------------------------------------------------------------------*
 * @brief In a distributed context, this function determines what the limit would be
 * for this local node. It does this be distributing and collecting the total number of 
 * rows in the table. Then knowing which node index this local node is, it can calculate
 * how many rows are ahead of the ones in this partition
 *
 * @param[in] contex
 * @param[in] local_num_rows    Number of rows of this partition
 * @param[in] limit_rows        Limit being applied to the whole query
 *
 * @returns The limit that would be applied to this partition
 *---------------------------------------------------------------------------**/
int64_t determine_local_limit(Context * context, int64_t local_num_rows, cudf::size_type limit_rows);

// BATCH FUNCTIONS

std::unique_ptr<ral::frame::BlazingTable> sort(const ral::frame::BlazingTableView & table, const std::string & query_part);
std::unique_ptr<ral::frame::BlazingTable> sample(const ral::frame::BlazingTableView & table, const std::string & query_part);
std::unique_ptr<ral::frame::BlazingTable> generate_distributed_partition_plan(const ral::frame::BlazingTableView & selfSamples, unsigned long long table_num_rows, unsigned long long avg_bytes_per_row, const std::string & query_part, Context * context);
std::unique_ptr<ral::frame::BlazingTable> generate_partition_plan(cudf::size_type number_partitions, const std::vector<ral::frame::BlazingTableView> & samples, const std::vector<size_t> & total_rows_tables, const std::string & query_part);
std::vector<cudf::table_view> partition_table(const ral::frame::BlazingTableView & partitionPlan, const ral::frame::BlazingTableView & sortedTable, const std::string & query_part);
std::vector<std::pair<int, std::unique_ptr<ral::frame::BlazingTable>>>
distribute_table_partitions(const ral::frame::BlazingTableView & partitionPlan,	const ral::frame::BlazingTableView & sortedTable, const std::string & query_part,	Context * context);
bool has_limit_only(const std::string & query_part);
int64_t get_local_limit(int64_t total_batch_rows, const std::string & query_part, Context * context);
std::pair<std::unique_ptr<ral::frame::BlazingTable>, int64_t>
limit_table(std::unique_ptr<ral::frame::BlazingTable> table, int64_t num_rows_limit);


// END BATCH FUNCTIONS

std::unique_ptr<ral::frame::BlazingTable> merge(std::vector<ral::frame::BlazingTableView> partitions_to_merge, const std::string & query_part);

}  // namespace experimental
}  // namespace operators
}  // namespace ral

#endif  // BLAZINGDB_RAL_ORDERBY_OPERATOR_H
