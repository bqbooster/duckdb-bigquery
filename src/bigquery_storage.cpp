#include "duckdb.hpp"

#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/parser/parsed_data/attach_info.hpp"
#include "duckdb/transaction/transaction_manager.hpp"

#include "bigquery_secret.hpp"
#include "bigquery_storage.hpp"
#include "storage/bigquery_catalog.hpp"
#include "storage/bigquery_options.hpp"
#include "storage/bigquery_transaction.hpp"

#include <iostream>

namespace duckdb {
namespace bigquery {


static unique_ptr<Catalog> BigqueryAttach(StorageExtensionInfo *storage_info,
                                          ClientContext &context,
                                          AttachedDatabase &db,
                                          const string &name,
                                          AttachInfo &info,
                                          AccessMode access_mode) {
    BigqueryOptions options;
    options.access_mode = access_mode;
	auto authInfo = BuildAuthenticationInfo(context, info);
    return duckdb::make_uniq<BigqueryCatalog>(db, info.path, options, std::move(authInfo));
}


static unique_ptr<TransactionManager> BigqueryCreateTransactionManager(StorageExtensionInfo *storage_info,
                                                                       AttachedDatabase &db,
                                                                       Catalog &catalog) {
    auto &bigquery_catalog = catalog.Cast<BigqueryCatalog>();
    return make_uniq<BigqueryTransactionManager>(db, bigquery_catalog);
}

BigqueryStorageExtension::BigqueryStorageExtension() {
    attach = BigqueryAttach;
    create_transaction_manager = BigqueryCreateTransactionManager;
}


} // namespace bigquery
} // namespace duckdb
