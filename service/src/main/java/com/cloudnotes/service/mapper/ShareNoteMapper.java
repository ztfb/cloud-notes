package com.cloudnotes.service.mapper;

import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.ShareRelation;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface ShareNoteMapper extends JpaRepository<ShareRelation,Integer> {
    public List<ShareRelation> findShareRelationsByNoteId(Integer nid);
    public List<ShareRelation> findShareRelationsByShareId(Integer sid);
    @Modifying
    @Query(value = "delete from share_relation where user_id=:uid",nativeQuery=true)
    public int deleteByUserId(@Param("uid") Integer uid);
}
