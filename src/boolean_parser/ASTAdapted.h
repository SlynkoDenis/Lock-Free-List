#ifndef LOCK_FREE_LIST_AST_ADAPTED_H_
#define LOCK_FREE_LIST_AST_ADAPTED_H_

#include "AST.h"
#include <boost/fusion/include/adapt_struct.hpp>


// namespace boost {

// namespace fusion {

// namespace traits {

// template< > struct tag_of<parser::ast::UnaryOp > { typedef struct_tag type; };

// template< > struct tag_of<parser::ast::UnaryOp const> { typedef struct_tag type; }; 

// }

// namespace extension {

// template< > struct access::struct_member< parser::ast::UnaryOp , 0 > {
//     struct deduced_attr_type {
//         static const parser::ast::UnaryOp& obj;
//         typedef boost::type_of::remove_cv_ref_t<decltype(obj.op)> type;
//     };

//     typedef deduced_attr_type::type attribute_type;

//     typedef attribute_type type;

//     template<typename Seq> struct apply {
//         typedef typename add_reference< typename mpl::eval_if< is_const<Seq> , add_const<attribute_type> , mpl::identity<attribute_type> >::type >::type type;
//         constexpr static type call(Seq& seq) { return seq. op; }
//     };
// };

// template< > struct struct_member_name< parser::ast::UnaryOp , 0 > {
//     typedef char const* type;
//     constexpr static type call() { return "op"; } };
    
//     template< > struct access::struct_member< parser::ast::UnaryOp , 1 > {
//         struct deduced_attr_type {
//             static const parser::ast::UnaryOp& obj;
//             typedef boost::type_of::remove_cv_ref_t<decltype(obj.rhs)> type;
//         };
        
//         typedef deduced_attr_type::type attribute_type;
//         typedef attribute_type type;
        
//         template<typename Seq> struct apply {
//             typedef typename add_reference< typename mpl::eval_if< is_const<Seq> , add_const<attribute_type> , mpl::identity<attribute_type> >::type >::type type;
//             constexpr static type call(Seq& seq) { return seq. rhs; }
//         };
//     };
//     template< > struct struct_member_name< parser::ast::UnaryOp , 1 > {
//         typedef char const* type;
//         constexpr static type call() { return "rhs"; } };

//         template< > struct struct_size<parser::ast::UnaryOp> : mpl::int_<2> {};
        
//         template< > struct struct_is_view< parser::ast::UnaryOp > : mpl::false_ {};
// }

// }

// namespace mpl {

// template<typename> struct sequence_tag;

// template< > struct sequence_tag<parser::ast::UnaryOp> {
//     typedef fusion::fusion_sequence_tag type; 
// };

// template< > struct sequence_tag< parser::ast::UnaryOp const > {
//     typedef fusion::fusion_sequence_tag type;
// };

// }

// }


BOOST_FUSION_ADAPT_STRUCT(parser::ast::UnaryOp, op, rhs)

BOOST_FUSION_ADAPT_STRUCT(parser::ast::BinaryOp, op, lhs, rhs)

BOOST_FUSION_ADAPT_STRUCT(parser::ast::Operation, op, rhs)

BOOST_FUSION_ADAPT_STRUCT(parser::ast::Expression, lhs, rhs)

#endif  // LOCK_FREE_LIST_AST_ADAPTED_H_
