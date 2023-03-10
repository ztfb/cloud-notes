package com.cloudnotes.service.mapper;

import com.cloudnotes.service.entity.TextBlock;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface TextBlockMapper extends JpaRepository<TextBlock,Integer> {
    public TextBlock findTextBlockByNoteId(Integer nid);
    @Modifying
    @Query(value = "delete from text_block where user_id=:uid;",nativeQuery=true)
    public int deleteByUserId(@Param("uid") Integer uid);
    public List<TextBlock> findTextBlocksByNoteId(Integer nid);
}
