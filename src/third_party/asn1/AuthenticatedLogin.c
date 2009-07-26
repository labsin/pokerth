/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "POKERTH-PROTOCOL"
 * 	found in "../../../docs/pokerth.asn1"
 */

#include <asn_internal.h>

#include "AuthenticatedLogin.h"

static asn_TYPE_member_t asn_MBR_AuthenticatedLogin_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AuthenticatedLogin, playerName),
		(ASN_TAG_CLASS_UNIVERSAL | (12 << 2)),
		0,
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"playerName"
		},
	{ ATF_POINTER, 1, offsetof(struct AuthenticatedLogin, avatar),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_AvatarHash,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"avatar"
		},
};
static ber_tlv_tag_t asn_DEF_AuthenticatedLogin_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_AuthenticatedLogin_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, 0, 0 }, /* avatar at 92 */
    { (ASN_TAG_CLASS_UNIVERSAL | (12 << 2)), 0, 0, 0 } /* playerName at 91 */
};
static asn_SEQUENCE_specifics_t asn_SPC_AuthenticatedLogin_specs_1 = {
	sizeof(struct AuthenticatedLogin),
	offsetof(struct AuthenticatedLogin, _asn_ctx),
	asn_MAP_AuthenticatedLogin_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_AuthenticatedLogin = {
	"AuthenticatedLogin",
	"AuthenticatedLogin",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_AuthenticatedLogin_tags_1,
	sizeof(asn_DEF_AuthenticatedLogin_tags_1)
		/sizeof(asn_DEF_AuthenticatedLogin_tags_1[0]), /* 1 */
	asn_DEF_AuthenticatedLogin_tags_1,	/* Same as above */
	sizeof(asn_DEF_AuthenticatedLogin_tags_1)
		/sizeof(asn_DEF_AuthenticatedLogin_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_AuthenticatedLogin_1,
	2,	/* Elements count */
	&asn_SPC_AuthenticatedLogin_specs_1	/* Additional specs */
};

