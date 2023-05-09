#ifndef LOCK_FREE_LIST_LOGGING_UTILS_H_
#define LOCK_FREE_LIST_LOGGING_UTILS_H_

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <fstream>

namespace expr = boost::log::expressions;

namespace lf::tests {
using text_sink = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;

void configureLogger(const std::string &filename) {
    auto sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(filename));
    sink->set_filter(boost::log::expressions::attr<std::string>("Channel") == filename);
    sink->set_formatter(
        expr::stream
            << '['
            << expr::attr<boost::posix_time::ptime>("TimeStamp")
            << "] [TID "
            << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
            << "]: "
            << expr::smessage);
    boost::log::core::get()->add_sink(sink);
    boost::log::add_common_attributes();
}
}   // namespace lf::tests

#endif  // LOCK_FREE_LIST_LOGGING_UTILS_H_
