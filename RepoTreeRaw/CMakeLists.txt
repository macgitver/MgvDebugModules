PROJECT( MGV_MOD_REPOTREERAW )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_REPOTREERAW_SOURCE_DIR}
    ${MGV_MOD_REPOTREERAW_BINARY_DIR}
)


SET(SRC_FILES
    RepoTreeRawModule.cpp
    RepoTreeRawModel.cpp
    RepoTreeRawView.cpp
)

SET(HDR_FILES
    RepoTreeRawModule.hpp
    RepoTreeRawModel.hpp
    RepoTreeRawView.hpp
)

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    RepoTreeRaw

    ${SRC_FILES}
    ${HDR_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${MOC_FILES}
    ${HIC_FILES} ${HID_FILES}

    Module.json
)

MSVC_SPLIT_SOURCES( ModRepoTreeRaw )
